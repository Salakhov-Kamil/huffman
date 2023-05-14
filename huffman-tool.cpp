#include "Coder.h"
#include "Decoder.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  std::vector<std::string> args(argv + 1, argv + argc);

  bool help = std::find(args.begin(), args.end(), "--help") != args.end();

  if (help) {
    std::cout << "Basic version. Use --compress and --decompress." << std::endl;
    return 0;
  }

  bool compress =
      std::find(args.begin(), args.end(), "--compress") != args.end();
  bool decompress =
      std::find(args.begin(), args.end(), "--decompress") != args.end();

  if (compress && decompress) {
    std::cerr
        << R"(Wrong args. Use only one of "--compress" and "--decompress")"
        << std::endl;
    return 1;
  }

  if (!compress && !decompress) {
    std::cerr << R"(Wrong args. Use "--compress", "--decompress" or "--help")"
              << std::endl;
    return 1;
  }

  if (argc != 1 + compress * 5 + decompress * 5 + help) {
    std::cerr << "Wrong count of args." << std::endl;
    return 1;
  }

  auto it = std::find(args.begin(), args.end(), "--input");
  if (it == args.end()) {
    std::cerr << R"(Don't use "--compress" or '--decompress' without "--input")"
              << std::endl;
    return 1;
  }

  ++it;
  if (it == args.end()) {
    std::cerr
        << R"(Don't use "--compress" or '--decompress' without input file)"
        << std::endl;
    return 1;
  }

  std::string inputFilename = *it;
  std::ifstream in(inputFilename, std::ifstream::binary);

  if (!in) {
    std::cerr << "Can't open file \'" + inputFilename + "\'" << std::endl;
    return 1;
  }

  it = std::find(args.begin(), args.end(), "--output");
  if (it == args.end()) {
    std::cerr
        << R"(Don't use "--compress" or '--decompress' without "--output")"
        << std::endl;
    return 1;
  }

  ++it;
  if (it == args.end()) {
    std::cerr
        << R"(Don't use "--compress" or '--decompress' without output file)"
        << std::endl;
    return 1;
  }

  std::string outputFilename = *it;
  std::ofstream out(outputFilename, std::ifstream::binary);

  if (!out) {
    std::cerr << "Can't create/open output file \'" + outputFilename + "\'"
              << std::endl;
    return 1;
  }

  if (compress) {
    huffman::Coder coder(in, out);
    try {
      coder.encode();
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
      return 1;
    }
  } else {
    huffman::Decoder decoder(in, out);
    try {
      decoder.decode();
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
      return 1;
    }
  }

  in.close();
  out.close();

  return 0;
}