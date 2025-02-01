#include "../include/ccompra/all.h"
#include <stdio.h>
#include <assert.h>

void rununit(void) {
    const char *input = "AAAABBBCCDDEEEEEFGGGHHHHIIIIJJJKKLLLMMMNNN";

    {
        char *lz77c = lz77_compress(input);
        char *lz77d = lz77_decompress(lz77c);
        assert(strcmp(input, lz77d) == 0 && "LZ77 decompression failed!");
        free(lz77c); free(lz77d);
        printf("LZ77:    Passed!\n");
    }

    {
        char *lz78c = lz78_compress(input);
        char *lz78d = lz78_decompress(lz78c);
        assert(strcmp(input, lz78d) == 0 && "LZ78 decompression failed!");
        free(lz78c); free(lz78d);
        printf("LZ78:    Passed!\n");
    }
    
    {
        char *lzmac = lzma_compress(input);
        char *lzmad = lzma_decompress(lzmac);
        assert(strcmp(input, lzmad) == 0 && "LZMA decompression failed!");
        free(lzmac); free(lzmad);
        printf("LZMA:    Passed!\n");
    }
    
    {
        HuffmanCompressed *huffc = huffman_compress(input);
        char *huffd = huffman_decompress(huffc);
        assert(strcmp(input, huffd) == 0 && "Huffman decompression failed!");
        free(huffc); free(huffd);
        printf("Huffman: Passed!\n");
    }

    {
        HuffmanCompressed *deflatec = deflate_compress(input);
        char *deflated = deflate_decompress(deflatec);
        assert(strcmp(input, deflated) == 0 && "Deflate decompression failed!");
        free(deflatec); free(deflated);
        printf("Deflate: Passed!\n");
    }

    {
        char *lz4c = lz4_compress(input);
        char *lz4d = lz4_decompress(lz4c);
        assert(strcmp(input, lz4d) == 0 && "LZ4 decompression failed!");
        free(lz4c); free(lz4d);
        printf("LZ4:     Passed!\n");
    }
   
    {
        char *lz5c = lz5_compress(input);
        char *lz5d = lz5_decompress(lz5c);
        assert(strcmp(input, lz5d) == 0 && "LZ5 decompression failed!");
        free(lz5c); free(lz5d);
        printf("LZ5:     Passed!\n");
    }

    {
        char *lzoc = lzo_compress(input);
        char *lzod = lzo_decompress(lzoc);
        assert(strcmp(input, lzod) == 0 && "LZO decompression failed!");
        free(lzoc); free(lzod);
        printf("LZO:     Passed!\n");
    }

    {
        LZWResult lzwRes = lzw_compress(input);
        char *lzwc = serializeLZWResult(lzwRes);
        char *lzwsd = lzw_decompress(lzwRes);
        assert(strcmp(input, lzwsd) == 0 && "LZW decompression failed!");
        free(lzwRes.codes); free(lzwc); free(lzwsd);
        printf("LZW:     Passed!\n");
    }

    {
        char *lzssc = lzss_compress(input, 128);
        char *lzssd = lzss_decompress(lzssc);
        assert(strcmp(input, lzssd) == 0 && "LZSS decompression failed!");
        free(lzssc); free(lzssd);
        printf("LZSS:    Passed!\n");
    }
    
    {
        FSECompressed *fsec = fse_compress(input);
        char *fsed = fse_decompress(fsec);
        assert(strcmp(input, fsed) == 0 && "FSE decompression failed!");
        free(fsec); free(fsed);
        printf("FSE:     Passed!\n");
    }
    
    {
        FSECompressed *zstdc = zstd_compress(input);
        char *zstdd = zstd_decompress(zstdc);
        assert(strcmp(input, zstdd) == 0 && "Zstandard decompression failed!");
        free(zstdc); free(zstdd);
        printf("ZSTD:    Passed!\n");
    }
}