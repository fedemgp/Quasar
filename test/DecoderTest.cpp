/**
 * Created by Federico Manuel Gomez Peter 
 * on 22/12/20.
 */
#include <gtest/gtest.h>
#include <vector>

#include "Decoder.h"

TEST(getLocation, returnsThePositionOfTheTransmitter) {
    Decoder decoder;
    decoder.addSatelite(KENOBI_NAME, {-500, -200});
    decoder.addSatelite(SKYWALKER_NAME, {100, -100});
    decoder.addSatelite(SATO_NAME, {500, 100});
    std::map<std::string, float> distances = {{KENOBI_NAME, 100}, {SKYWALKER_NAME, 115.5}, {SATO_NAME, 142.7}};
    const Position transmitterPos = decoder.getLocation(distances);
    EXPECT_NEAR(transmitterPos.x, -487.28, 0.01);
    EXPECT_NEAR(transmitterPos.y, 1557.01, 0.01);
}

TEST(getLocation, anotherTrueAssert) {
    Decoder decoder;
    decoder.addSatelite(KENOBI_NAME, {5, 15});
    decoder.addSatelite(SKYWALKER_NAME, {5, 5});
    decoder.addSatelite(SATO_NAME, {20, 10});
    std::map<std::string, float> distances = {{KENOBI_NAME, 5.65}, {SKYWALKER_NAME, 7.21}, {SATO_NAME, 11.04}};
    const Position transmitterPos = decoder.getLocation(distances);
    EXPECT_NEAR(transmitterPos.x, 9, 0.1);
    EXPECT_NEAR(transmitterPos.y, 11, 0.1);
}

TEST(getLocation, estimatesToEquidistantPointIfCirclesDoesntIntersect) {
    Decoder decoder;
    decoder.addSatelite(KENOBI_NAME, {5, 15});
    decoder.addSatelite(SKYWALKER_NAME, {5, 5});
    decoder.addSatelite(SATO_NAME, {20, 10});
    // Los circulos no se intersectan, el punto medio entre estos es (9; 9.5)
    std::map<std::string, float> distances = {{KENOBI_NAME, 5}, {SKYWALKER_NAME, 4}, {SATO_NAME, 10}};
    Position transmitterPos = decoder.getLocation(distances);
    EXPECT_NEAR(transmitterPos.x, 9, 0.1);
    EXPECT_NEAR(transmitterPos.y, 9.5, 0.1);
    // El resultado cuando las distancias son las correctas
    distances = {{KENOBI_NAME, 6.8}, {SKYWALKER_NAME, 6}, {SATO_NAME, 11}};
    transmitterPos = decoder.getLocation(distances);
    EXPECT_NEAR(transmitterPos.x, 9, 0.1);
    EXPECT_NEAR(transmitterPos.y, 9.5, 0.1);
}

TEST(getLocation, estimatesToEquidistantPointIfCirclesDoesntIntersectInASinglePoint) {
    Decoder decoder;
    decoder.addSatelite(KENOBI_NAME, {5, 15});
    decoder.addSatelite(SKYWALKER_NAME, {5, 5});
    decoder.addSatelite(SATO_NAME, {20, 10});
    // Los circulos se intersectan, pero no hay convergencia en un punto, sinó en un area cuyo centro es (9; 9.5)
    std::map<std::string, float> distances = {{KENOBI_NAME, 7.8}, {SKYWALKER_NAME, 7}, {SATO_NAME, 11.5}};
    Position transmitterPos = decoder.getLocation(distances);
    EXPECT_NEAR(transmitterPos.x, 9, 0.1);
    EXPECT_NEAR(transmitterPos.y, 9.5, 0.1);
    // El resultado cuando las distancias son las correctas
    distances = {{KENOBI_NAME, 6.8}, {SKYWALKER_NAME, 6}, {SATO_NAME, 11}};
    transmitterPos = decoder.getLocation(distances);
    EXPECT_NEAR(transmitterPos.x, 9, 0.1);
    EXPECT_NEAR(transmitterPos.y, 9.5, 0.1);
}

TEST(getString, decodesMessage) {
    Decoder decoder;
    std::vector<web::json::array> messages;
    messages.emplace_back(web::json::value::parse("[\"\", \"hola\", \"\"]").as_array());
    messages.emplace_back(web::json::value::parse("[\"hola\", \"\"]").as_array());
    messages.emplace_back(web::json::value::parse("[\"\", \"\", \"\", \"mundo\"]").as_array());
    std::string result = decoder.getString(messages);
    EXPECT_EQ(result, "hola mundo");
}

TEST(getString, throwsExceptionIfAWordCouldntBeDecoded) {
    try {
        Decoder decoder;
        std::vector<web::json::array> messages;
        messages.emplace_back(web::json::value::parse("[\"un\", \"mensaje\", \"\"]").as_array());
        messages.emplace_back(web::json::value::parse("[\"un\", \"mensaje\", \"\"]").as_array());
        messages.emplace_back(web::json::value::parse("[\"un\", \"mensaje\", \"\"]").as_array());

        std::string result = decoder.getString(messages);
    } catch (std::exception &e) {
        EXPECT_EQ(std::string(e.what()),
    "Error when decoding message: word can't be found. Partial result = un mensaje ");
    } catch (...) {
        FAIL() << "Expected: \"Error when decoding message: word can't be found. Partial result = un mensaje \"";
    }
}