/**
 * Created by Federico Manuel Gomez Peter 
 * on 20/12/20.
 */
#include <gtest/gtest.h>
#include <vector>

#include "Decoder.h"

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(getLocation, returnsThePositionOfTheTransmitter) {
    std::vector<Position> satellites;
    satellites.emplace_back(-500, -200);
    satellites.emplace_back(100, -100);
    satellites.emplace_back(500, 100);
    Decoder decoder(satellites);
    std::vector<float> distances = {100, 115.5, 142.7};
    const Position transmitterPos = decoder.getLocation(distances);
    EXPECT_NEAR(transmitterPos.x, -487.28, 0.01);
    EXPECT_NEAR(transmitterPos.y, 1557.01, 0.01);
}

TEST(getLocation, anotherTrueAssert) {
    std::vector<Position> satellites;
    satellites.emplace_back(5, 15);
    satellites.emplace_back(5, 5);
    satellites.emplace_back(20, 10);
    Decoder decoder(satellites);
    std::vector<float> distances = {5.65, 7.21, 11.04};
    const Position transmitterPos = decoder.getLocation(distances);
    EXPECT_NEAR(transmitterPos.x, 9, 0.1);
    EXPECT_NEAR(transmitterPos.y, 11, 0.1);
}

TEST(getLocation, estimatesToEquidistantPointIfCirclesDoesntIntersect) {
    std::vector<Position> satellites;
    satellites.emplace_back(5, 15);
    satellites.emplace_back(5, 5);
    satellites.emplace_back(20, 10);
    Decoder decoder(satellites);
    // Los circulos no se intersectan, el punto medio entre estos es (9; 9.5)
    std::vector<float> distances = {5, 4, 10};
    Position transmitterPos = decoder.getLocation(distances);
    EXPECT_NEAR(transmitterPos.x, 9, 0.1);
    EXPECT_NEAR(transmitterPos.y, 9.5, 0.1);
    // El resultado cuando las distancias son las correctas
    distances = {6.8, 6, 11};
    transmitterPos = decoder.getLocation(distances);
    EXPECT_NEAR(transmitterPos.x, 9, 0.1);
    EXPECT_NEAR(transmitterPos.y, 9.5, 0.1);
}

TEST(getLocation, estimatesToEquidistantPointIfCirclesDoesntIntersectInASinglePoint) {
    std::vector<Position> satellites;
    satellites.emplace_back(5, 15);
    satellites.emplace_back(5, 5);
    satellites.emplace_back(20, 10);
    Decoder decoder(satellites);
    // Los circulos se intersectan, pero no hay convergencia en un punto, sinó en un area cuyo centro es (9; 9.5)
    std::vector<float> distances = {7.8, 7, 11.5};
    Position transmitterPos = decoder.getLocation(distances);
    EXPECT_NEAR(transmitterPos.x, 9, 0.1);
    EXPECT_NEAR(transmitterPos.y, 9.5, 0.1);
    // El resultado cuando las distancias son las correctas
    distances = {6.8, 6, 11};
    transmitterPos = decoder.getLocation(distances);
    EXPECT_NEAR(transmitterPos.x, 9, 0.1);
    EXPECT_NEAR(transmitterPos.y, 9.5, 0.1);
}

TEST(getString, decodesMessage) {
    std::vector<Position> satellites = {{5, 15}, {5, 5}, {20, 10}};
    Decoder decoder(satellites);
    std::vector<std::vector<std::string>> messages = {{"", "hola", ""},{"hola",""},{"", "", "", "mundo"}};
    std::string result = decoder.getString(messages);
    EXPECT_EQ(result, "hola mundo");
}

TEST(getString, throwsExceptionIfAWordCouldntBeDecoded) {
    try {
        std::vector<Position> satellites = {{5, 15}, {5, 5}, {20, 10}};
        Decoder decoder(satellites);
        std::vector<std::vector<std::string>> messages = {{"un", "mensaje", ""},{"un","mensaje", ""},{"un", "mensaje", ""}};
        std::string result = decoder.getString(messages);
    } catch (std::exception &e) {
        EXPECT_EQ(std::string(e.what()),
                  "Error when decoding message: word can't be found. Partial result = un mensaje ");
    } catch (...) {
        FAIL() << "Expected: \"Error when decoding message: word can't be found. Partial result = un mensaje \"";
    }
}