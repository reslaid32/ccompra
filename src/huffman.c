#define CCOMPRA_LIBRARY_BUILD

#include "../include/ccompra/huffman.h"

typedef struct HuffmanNode {
    char ch;
    int freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;

CCOMPRA_PRIVATE_FUNC
HuffmanNode* createHuffmanNode(char ch, int freq) {
    HuffmanNode *node = malloc(sizeof(HuffmanNode));
    node->ch = ch;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

CCOMPRA_PRIVATE_FUNC
void freeHuffmanTree(HuffmanNode *node) {
    if (!node) return;
    freeHuffmanTree(node->left);
    freeHuffmanTree(node->right);
    free(node);
}

CCOMPRA_PRIVATE_FUNC
void buildFrequencyTable(const char *text, int freq[256]) {
    for (int i = 0; i < 256; i++)
        freq[i] = 0;
    for (const char *p = text; *p; p++)
        freq[(unsigned char)*p]++;
}

CCOMPRA_PRIVATE_FUNC
char* serializeFrequencyTable(const int freq[256]) {
    // Allocate a buffer for the frequency table string.
    char *str = malloc(1024);
    str[0] = '\0';
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            char buf[32];
            snprintf(buf, sizeof(buf), "%d:%d,", i, freq[i]);
            strcat(str, buf);
        }
    }
    return str;
}

CCOMPRA_PRIVATE_FUNC
HuffmanNode* buildHuffmanTree(const char *text) {
    int freq[256];
    buildFrequencyTable(text, freq);
    HuffmanNode *nodes[256];
    int count = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0)
            nodes[count++] = createHuffmanNode((char)i, freq[i]);
    }
    if (count == 0) return NULL;
    
    while (count > 1) {
        int min1 = 0, min2 = 1;
        if (nodes[min2]->freq < nodes[min1]->freq) {
            int tmp = min1;
            min1 = min2;
            min2 = tmp;
        }
        for (int i = 2; i < count; i++) {
            if (nodes[i]->freq < nodes[min1]->freq) {
                min2 = min1;
                min1 = i;
            } else if (nodes[i]->freq < nodes[min2]->freq) {
                min2 = i;
            }
        }
        HuffmanNode *left = nodes[min1];
        HuffmanNode *right = nodes[min2];
        HuffmanNode *parent = createHuffmanNode('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        
        // Remove nodes[min1] and nodes[min2] from array (remove higher index first)
        if (min1 > min2) {
            int tmp = min1;
            min1 = min2;
            min2 = tmp;
        }
        for (int i = min2; i < count - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        count--;
        for (int i = min1; i < count - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        count--;
        nodes[count++] = parent;
    }
    return nodes[0];
}

CCOMPRA_PRIVATE_FUNC
void generateHuffmanCodes(HuffmanNode *node, char *code, int depth, char codes[256][256]) {
    if (!node) return;
    if (!node->left && !node->right) {
        code[depth] = '\0';
        strcpy(codes[(unsigned char)node->ch], code);
        return;
    }
    if (node->left) {
        code[depth] = '0';
        generateHuffmanCodes(node->left, code, depth+1, codes);
    }
    if (node->right) {
        code[depth] = '1';
        generateHuffmanCodes(node->right, code, depth+1, codes);
    }
}

CCOMPRA_PRIVATE_FUNC
unsigned char* packBits(const char *bitString, int bitLen, int *outByteLen) {
    int byteLen = (bitLen + 7) / 8;
    unsigned char *bytes = malloc(byteLen);
    memset(bytes, 0, byteLen);
    for (int i = 0; i < bitLen; i++) {
        if (bitString[i] == '1')
            bytes[i/8] |= (1 << (7 - (i % 8)));
    }
    *outByteLen = byteLen;
    return bytes;
}

CCOMPRA_PRIVATE_FUNC
char* unpackBits(const unsigned char *bytes, int bitLen) {
    char *bitString = malloc(bitLen + 1);
    for (int i = 0; i < bitLen; i++) {
        int byteIndex = i / 8;
        int bitIndex = 7 - (i % 8);
        bitString[i] = (bytes[byteIndex] & (1 << bitIndex)) ? '1' : '0';
    }
    bitString[bitLen] = '\0';
    return bitString;
}

CCOMPRA_PUBLIC_FUNC
HuffmanCompressed* huffman_compress(const char *text) {
    // Build frequency table and frequency string header.
    int freq[256];
    buildFrequencyTable(text, freq);
    char *freqStr = serializeFrequencyTable(freq);

    // Build the Huffman tree and generate codes.
    HuffmanNode *root = buildHuffmanTree(text);
    char codes[256][256] = {{0}};
    char code[256] = "";
    generateHuffmanCodes(root, code, 0, codes);

    // Build the bit string using the Huffman codes.
    size_t cap = 1024;
    char *bitString = malloc(cap);
    bitString[0] = '\0';
    for (const char *p = text; *p; p++) {
        const char *c = codes[(unsigned char)*p];
        if (strlen(bitString) + strlen(c) + 1 > cap) {
            cap *= 2;
            bitString = realloc(bitString, cap);
        }
        strcat(bitString, c);
    }
    int bitLen = (int)strlen(bitString);

    // Pack the bit string into bytes.
    int dataLen = 0;
    unsigned char *packedData = packBits(bitString, bitLen, &dataLen);

    // Free temporary allocations.
    free(bitString);
    freeHuffmanTree(root);

    // Allocate and fill the result structure.
    HuffmanCompressed *result = malloc(sizeof(HuffmanCompressed));
    result->freqStr = freqStr; // already allocated by serializeFrequencyTable
    result->bitLen = bitLen;
    result->data = packedData;
    result->dataLen = dataLen;
    return result;
}

CCOMPRA_PUBLIC_FUNC
char* huffman_decompress(const HuffmanCompressed *compressed) {
    // Duplicate the frequency table string since strtok modifies its input.
    char *freqStr = strdup(compressed->freqStr);

    // Rebuild the frequency table from the header.
    int freq[256] = {0};
    char *freqCopy = strdup(freqStr);
    char *pair = strtok(freqCopy, ",");
    while (pair) {
        int ch, f;
        if (sscanf(pair, "%d:%d", &ch, &f) == 2)
            freq[ch] = f;
        pair = strtok(NULL, ",");
    }
    free(freqCopy);
    free(freqStr);

    // Compute the total number of characters.
    int textLen = 0;
    for (int i = 0; i < 256; i++)
        textLen += freq[i];

    // Rebuild the Huffman tree from the frequency table.
    HuffmanNode *nodes[256];
    int count = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0)
            nodes[count++] = createHuffmanNode((char)i, freq[i]);
    }
    if (count == 0) {
        // No data.
        return strdup("");
    }
    while (count > 1) {
        int min1 = 0, min2 = 1;
        if (nodes[min2]->freq < nodes[min1]->freq) {
            int tmp = min1;
            min1 = min2;
            min2 = tmp;
        }
        for (int i = 2; i < count; i++) {
            if (nodes[i]->freq < nodes[min1]->freq) {
                min2 = min1;
                min1 = i;
            } else if (nodes[i]->freq < nodes[min2]->freq) {
                min2 = i;
            }
        }
        HuffmanNode *left = nodes[min1];
        HuffmanNode *right = nodes[min2];
        HuffmanNode *parent = createHuffmanNode('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        
        if (min1 > min2) {
            int tmp = min1;
            min1 = min2;
            min2 = tmp;
        }
        for (int i = min2; i < count - 1; i++) {
            nodes[i] = nodes[i+1];
        }
        count--;
        for (int i = min1; i < count - 1; i++) {
            nodes[i] = nodes[i+1];
        }
        count--;
        nodes[count++] = parent;
    }
    HuffmanNode *root = nodes[0];

    // Unpack the binary data back into a bit string.
    char *bitString = unpackBits(compressed->data, compressed->bitLen);

    // Decode the bit string using the Huffman tree.
    char *result = malloc(textLen + 1);
    int resIndex = 0;
    HuffmanNode *cur = root;
    for (int i = 0; i < compressed->bitLen; i++) {
        cur = (bitString[i] == '0') ? cur->left : cur->right;
        if (!cur->left && !cur->right) {
            result[resIndex++] = cur->ch;
            cur = root;
            if (resIndex == textLen)
                break;
        }
    }
    result[resIndex] = '\0';

    free(bitString);
    freeHuffmanTree(root);
    return result;
}

CCOMPRA_PUBLIC_FUNC
size_t huffman_compress_bound(size_t srcSz) {
    if (srcSz == 0)
        return 1;  // At least one byte to represent an empty output.
    
    // Worst-case frequency table: assume every one of 256 possible symbols appears,
    // and each is encoded as a string pair (e.g. \"123:456,\") taking up to ~25 bytes.
    size_t freqBound = 256 * 25; // = 6400 bytes (overestimate)
    
    // Worst-case bit string:
    // When 256 symbols appear the worst-case Huffman code length can be 255 bits.
    // (A completely skewed tree with 256 leaves can have maximum code length 255.)
    size_t bitStringBound = srcSz * 255;
    
    // When packing bits into bytes, the size is rounded up.
    size_t packedDataBound = (bitStringBound + 7) / 8;
    
    // Total worst-case bound: frequency table + packed data.
    return freqBound + packedDataBound;
}