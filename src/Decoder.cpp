/**
 * Created by Federico Manuel Gomez Peter 
 * on 20/12/20.
 */

#include "Decoder.h"
#include "Exception.h"

Decoder::Decoder(std::vector<Position> &satelites): satelites(std::move(satelites)) {}

Decoder::~Decoder() {}
/**
 * Sabiendo la formula de los circulos de cada satelite:
 *
 * r1^2 = (x - x1)^2 + (y - y1)^2
 * r2^2 = (x - x2)^2 + (y - y2)^2
 * r3^2 = (x - x3)^2 + (y - y3)^2
 *
 * con ri la distancia entre el emisor y el statelite i, y (xi;yi) la
 * posición de los satelites en el sistema de ejes coordenados. La
 * posición del emisor se puede calcular como
 *
 * xe = (BF - CE) / (DB - AE)
 * ye = (AF - DC) / (AE - DB)
 *
 * siendo:
 *
 * A = 2 * x2 - 2 * x1
 * B = 2 * y2 - 2 * y1
 * C = r1² - r2² - x1² + x2² - y1² + y2²
 * D = 2 * x3 - 2 * x2
 * E = 2 * y3 - 2 * y2
 * F = r2² - r3² - x2² + x3² - y2² + y3²
 */
Position Decoder::getLocation(std::vector<float> &distances) const {
    float A, B, C, D, E, F;
    A = 2 * this->satelites[1].x - 2 * this->satelites[0].x;
    B = 2 * this->satelites[1].y - 2 * this->satelites[0].y;
    D = 2 * this->satelites[2].x - 2 * this->satelites[1].x;
    E = 2 * this->satelites[2].y - 2 * this->satelites[1].y;
    float r1s, r2s, r3s, x1s, x2s, x3s, y1s, y2s, y3s;
    r1s = distances[0] * distances[0];
    r2s = distances[1] * distances[1];
    r3s = distances[2] * distances[2];
    x1s = this->satelites[0].x * this->satelites[0].x;
    x2s = this->satelites[1].x * this->satelites[1].x;
    x3s = this->satelites[2].x * this->satelites[2].x;
    y1s = this->satelites[0].y * this->satelites[0].y;
    y2s = this->satelites[1].y * this->satelites[1].y;
    y3s = this->satelites[2].y * this->satelites[2].y;
    C = r1s - r2s - x1s + x2s - y1s + y2s;
    F = r2s - r3s - x2s + x3s - y2s + y3s;
    return Position((B*F -C*E) / (D*B - A*E), (A*F - D*C) / (A*E - D*B) );
}
/**
 * Sabiendo que cada emisor va a enviar un vector de tamaño N + di, siendo di el desfasaje del satelite
 * i, se puede obtener el tamaño del mensaje popeando los strings vacíos al principio de cada vector. Como puede pasar
 * de que algún satélite no haya podido decodifcar la primer palabra del mensaje, va a haber tamaños de vectores
 * menores que N. Se sabe que el tamaño máximo después de popear los strings vacios al comienzo va a ser N.
 * Con esto, appendeao al principio mensajes vacíos para que todos los vectores tengan el tamaño N.
 */
std::string Decoder::getString(std::vector<std::vector<std::string>> &messages) const {
    size_t messageSize = 0;
    for (auto &sateliteMsg: messages) {
        int i = 0;
        while(sateliteMsg[i] == "" ) {
            i++;
        }
        messageSize = std::max(messageSize, sateliteMsg.size() - i);
    }

    for (auto &msg: messages) {
        if(msg.size() > messageSize) {
            msg.erase(msg.begin(), msg.begin() + (msg.size() - messageSize));
        }
    }

    std::string result;
    for (size_t i = 0; i < messageSize; i++) {
        bool decodeFailed = true;
        for (auto &msg: messages) {
            if (msg[i] != "") {
                result += msg[i];
                // add space if it is not the last word
                if (i + 1 < messageSize)
                    result += " ";
                decodeFailed = false;
                break;
            }
        }
        if (decodeFailed) {
            throw Exception("Error when decoding message: word can't be found. Partial result = %s", result.c_str());
        }
    }
    return result;
}

