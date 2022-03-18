#include <catch.hpp>
#include <decoder.h>

#include <string>
#include <fstream>

std::string ReadText(const std::string& filename) {
    char current;
    std::string text;
    std::ifstream fin(filename);
    while (fin.get(current)) {
        text += current;
    }
    return text;
}

static auto dictionary = ReadNgrams("../decoder/english_quadgrams.txt");

TEST_CASE("my test1") {
    REQUIRE(Decode(ReadText("../decoder/tests/first.txt"), dictionary).substr(0, 41) ==
            "At the crisp command of Shark Dodson, who");

}

TEST_CASE("my test2") {
    REQUIRE(Decode(ReadText("../decoder/tests/second.txt"), dictionary).substr(0, 44) ==
            "Simulated annealing (SA) is a probabilistic ");
}