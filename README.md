# libccompra

## **libccompra is an independent lightweight library with support for many compression and decompression algorithms written in C language**

### Supported Algorithms

| **Family**                      | **Variants**                                               |
|---------------------------------|------------------------------------------------------------|
| **Lempel-Ziv**                  | `LZ77`, `LZ78`, `LZ4`, `LZ5`, `LZO`, `LZW`, `LZMA`, `LZSS` |
| **Huffman Coding**              | `Huffman`, `FSE`                                           |
| **Lempel-Ziv + Huffman Coding** | `Deflate`, `Zstandard (Zstd)`                              |


## Building
   - ### **Build**
   ```bash
   https://github.com/reslaid32/ccompra.git
   cd ccompra
   make all
   ```

   - ### **Testing**
   ```bash
   https://github.com/reslaid32/ccompra.git
   cd ccompra
   sudo make all install check
   ```

## Installing & Uninstalling
   - ### **Installing**
   ```bash
   git clone https://github.com/reslaid32/ccompra.git
   cd ccompra
   sudo make all install
   ```

   - ### **Removing a library**
   ```bash
   git clone https://github.com/reslaid32/ccompra.git
   cd ccompra
   sudo make uninstall
   ```
