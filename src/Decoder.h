/**
 * Created by Federico Manuel Gomez Peter 
 * on 20/12/20.
 */

#ifndef __DECODER_H__
#define __DECODER_H__

#include <vector>
#include <string>

struct Position {
    float x;
    float y;
    Position(float x, float y): x(x), y(y) {};
};

class Decoder {
private:
    std::vector<Position> satelites;
public:
    /**
     * Constructor del decodificador. Recibe la posición de los 3 satélites.
     */
    explicit Decoder(std::vector<Position> &satelites);
    ~Decoder();
    /**
     * Dada la distancias del emisor a cada satelite, devuelve la posición del emisor mediante el cálculo de
     * trilateración.
     *
     * @param distances: distancia entre el emisor y el satelite i-ésimo
     * @return Posición (x,y) del emisor
     */
    Position getLocation(std::vector<float> &distances) const;
    /**
     * Dada la lista de palabras decodificadas por cada emisor, se reconstruirá el mensaje orginal. Cada emisor
     * pudo no haber decodificado completamente el mensaje, las palabras que no fueron decodificadas correctamnete
     * se representarán como un string vacío. El vector además puede contener varios strings vacios al inicio
     * del vector, lo que representa un desfasaje del mensaje.
     *
     * @param messages: una matriz de strings, donde cada fila representa el mensaje que pudo decoficar el satelite i.
     * @return El mensaje reconstruido.
     */
    std::string getString(std::vector<std::vector<std::string>> &messages) const;
};


#endif //__DECODER_H__
