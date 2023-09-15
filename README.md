# Data Structures Huffman Project

This project was written for my CS3610 Data Structures class in Fall of 2022. The purpose of this project was to use Huffman code tree to encode and decode files. 

The only file I modified was huffman.cc. Other files were provided by Nasseef Abukamail (professor at Ohio University).

Project can be compiled with `make` and run with `./a.out` and the usage below:
```
Usage: [option] [input file] [output file]
[-e input.txt output.txt] (encodes entire input file and puts results in output file)
[-d input.txt output.txt] (decodes entire input file and puts results in output file)
[-p] (prints the codes of all letters and the average code length)
[-h] (prints the help menu)
```

`in.txt` includes text to be encoded and `out.txt` includes results to be decoded.