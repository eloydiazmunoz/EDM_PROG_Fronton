#include <iostream>
#include <SFML/Graphics.hpp>
#include "funcionsJoc.h"

using namespace sf;
using namespace std;


int main() {

    // Finestra
    Vector2f viewSize(x_max, y_max);
    VideoMode vm(viewSize.x, viewSize.y);
    RenderWindow window(vm, title, Style::Default);
    window.setFramerateLimit(60);
    CircleShape bola(radi);
    RectangleShape player;
    std::vector<Bloque> bloques;
    int puntuacio = 0;
    int temps = 4800;
    Clock Rellotje;

    // Definim com és la pilota
    crearmon(bola, player);
    InicialitzarBlocs(bloques);

    // Velocitat inicial de la pilota
    float ballSpeedX = 3.0f;
    float ballSpeedY = -3.0f;

    Clock cronometre;
    bool fijoc = false;
    while (window.isOpen() && !fijoc) {
        // Actualitzem l'entrada
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // Actualitzem l'entrada del jugador  
        float dt = cronometre.restart().asSeconds();
        EntradaJugador(window, player, dt);

        // Actualitzem el joc
        fijoc = Update(dt, bola, player, ballSpeedX, ballSpeedY, bloques,puntuacio,temps,Rellotje);

        // Dibuixem el joc
        Draw(window, bola, player, bloques, puntuacio,temps);
    }

    return 0;
}