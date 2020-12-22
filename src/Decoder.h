/**
 * Created by Federico Manuel Gomez Peter 
 * on 20/12/20.
 */

#ifndef __DECODER_H__
#define __DECODER_H__

#include <map>
#include <string>
#include <vector>
#include <cpprest/details/http_helpers.h>

#define KENOBI_NAME "kenobi"
#define SKYWALKER_NAME "skywalker"
#define SATO_NAME "sato"

struct Position {
    float x;
    float y;
    Position(float x, float y): x(x), y(y) {};
};

class Decoder {
private:
    std::map<std::string, Position> satelites;
public:
    Decoder();
    ~Decoder();
    void addSatelite(const std::string name, const Position intial);
    /**
     * Dada la distancias del emisor a cada satelite, devuelve la posición del emisor mediante el cálculo de
     * trilateración.
     *
     * @param distances: distancia entre el emisor y el satelite i-ésimo
     * @return Posición (x,y) del emisor
     */
    Position getLocation(std::map<std::string, float> &distances) const;
    /**
     * Dada la lista de palabras decodificadas por cada emisor, se reconstruirá el mensaje orginal. Cada emisor
     * pudo no haber decodificado completamente el mensaje, las palabras que no fueron decodificadas correctamnete
     * se representarán como un string vacío. El vector además puede contener varios strings vacios al inicio
     * del vector, lo que representa un desfasaje del mensaje.
     *
     * @param messages: una matriz de strings, donde cada fila representa el mensaje que pudo decoficar el satelite i.
     * @return El mensaje reconstruido.
     */
    std::string getString(std::vector<web::json::array> &messages) const;
};


#endif //__DECODER_H__
