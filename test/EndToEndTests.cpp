#include <sstream>
#include <iostream>

#include "doctest.h"
#include "HuffmanArchiver.hpp"


void test_text_file(const std::string &text) {
    std::stringstream my_file{text};
    std::stringstream my_file_compressed{};
    std::stringstream my_file_decompressed{};
    HuffmanArchiver::compress(my_file, my_file_compressed);
    HuffmanArchiver::extract(my_file_compressed, my_file_decompressed);
    for (char c : text) {
        REQUIRE_EQ(c, static_cast<char>(my_file_decompressed.get()));
    }
    REQUIRE_EQ(static_cast<char>(my_file_decompressed.get()), -1);
}


TEST_CASE ("Small text file") {
    std::string text = "Mom washing the frame";
    test_text_file(text);
}

TEST_CASE ("Medium text file") {
    std::string text = "Working with a team?\n"
                       "GitHub is built for collaboration. Set up an organization to improve "
                       "the way your team works together, and get access to more features. ";
    test_text_file(text);
}

TEST_CASE ("Large text file") {
    std::string text = "The Grand Canyon, one of the Seven Wonders of the Natural World, is located in the state of Arizona. It is "
                       "also a UNESCO World Heritage Site. Formed by over 70 million years of erosion from the Colorado River, the "
                       "Grand Canyon offers a spectacular view. The canyon spans 277 miles in length, up to 18 miles in width, and "
                       "it measures over a mile in depth at its deepest points. Carlos always wanted to visit the Grand Canyon, and"
                       " recently he received the chance to hike some of the trails and take several panoramic photographs during h"
                       "is visit.\n"
                       "\n"
                       "When Carlos arrived at the visitor center, he watched a brief movie that taught tourists about the Grand Can"
                       "yon National Park and the geological history of the canyon’s formation. Later, Carlos followed a hiking trai"
                       "l to become even further acquainted with the canyon. While walking through the trails, Carlos saw some touri"
                       "sts riding donkeys to traverse the canyon’s ridges. Throughout his hiking expedition, Carlos used a map to f"
                       "ind some of the best hot spots for photographs within the canyon. He revisited some areas at different point"
                       "s in the day because sun angles and lighting can make a big difference in the quality of a photo.\n"
                       "\n"
                       "Carlos was very pleased that he got to travel to the Grand Canyon. He loves hiking and photography, so this w"
                       "as the perfect outdoor experience for him. He posted all of his best pictures on social media, and his friend"
                       "s were amazed by his breathtaking, panoramic shots. ";
    test_text_file(text);
}

TEST_CASE ("Binary file file") {
    std::string file{};
    std::size_t len = rand() % 1000;
    for (std::size_t i = 0; i < len; ++i) {
        file.push_back(static_cast<char>(rand()));
    }
    test_text_file(file);
}

TEST_CASE ("One symbol text") {
    std::string text = "MMMMMMMMMMMMMM";
    test_text_file(text);
}

TEST_CASE ("Two symbols text") {
    std::string text = "MMMMMMMMMMMMMMee";
    test_text_file(text);
}