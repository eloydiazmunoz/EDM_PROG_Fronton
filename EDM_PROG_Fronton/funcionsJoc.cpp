#include "funcionsJoc.h"

void Draw(RenderWindow& finestre, CircleShape& bola, RectangleShape& player, std::vector<Bloque>& blocs, int punt,int temps) {
   
    finestre.clear(sf::Color::Cyan);
    finestre.draw(bola);
    finestre.draw(player);
    DibuixarBlocs(finestre, blocs);

    Font font;
    if (!font.loadFromFile("./fonts/KOMIKAP_.ttf")) {
        std::cerr << "No es va poder carregar la font" << std::endl;
        return;
    }

    Text puntuacioText;
    Text tempsText;

    puntuacioText.setFont(font);
    puntuacioText.setCharacterSize(24);
    puntuacioText.setFillColor(sf::Color::Black);
    puntuacioText.setPosition(10, 10);
    puntuacioText.setString("Puntuació: " + std::to_string(punt));

    tempsText.setFont(font);
    tempsText.setCharacterSize(24);
    tempsText.setFillColor(sf::Color::Black);
    tempsText.setPosition(x_max - 170, 10);
    tempsText.setString("Temps: " + std::to_string(temps));

    finestre.draw(puntuacioText);
    finestre.draw(tempsText);

    finestre.display();
}

bool Update(float dt, CircleShape& bola, RectangleShape& player, float& ballSpeedX, float& ballSpeedY, std::vector<Bloque>& blocs, int& punt,int& temps,Clock Rellotje) {
    bool resposta = false;
    // Actualitzem la posició de la pilota
    bola.move(ballSpeedX, ballSpeedY);

    // Comprovem la col·lisió amb les parets
    if (bola.getPosition().x - radi < x_min || bola.getPosition().x + radi > x_max) {
        ballSpeedX = -ballSpeedX;  // Inverteix la direcció en col·lisió amb les parets horitzontals
    }

    // Comprovem la col·lisió amb les parets superior i inferior
    if (bola.getPosition().y - radi < y_min || bola.getPosition().y + radi > y_max) {
        ballSpeedY = -ballSpeedY;  // Inverteix la direcció en col·lisió amb les parets verticals
    }

    // Comprovem la col·lisió amb el jugador
    if (bola.getPosition().y + radi > player.getPosition().y - amplada / 5 &&
        bola.getPosition().x + radi > player.getPosition().x - longitut / 5 &&
        bola.getPosition().x - radi < player.getPosition().x + longitut / 5) {
        ballSpeedY = -ballSpeedY;  // Inverteix la direcció en col·lisió amb el jugador
    }

    // Comprovem la condició de pèrdua del joc (pilota toca el límit inferior en l'eix Y)
    if (bola.getPosition().y + radi > y_max) {
        std::cout << "Has perdut!" << std::endl;
        resposta = true;
    }

    for (int fila = 0; fila < numBloquesFilas; ++fila) {
        for (int i = 0; i < numBloques; ++i) {
            int indice = fila * numBloques + i;
            if (blocs[indice].activo && ColisioAmbBlocs(bola, blocs[indice].shape)) {
                ballSpeedY = -ballSpeedY;
                blocs[indice].activo = false;

                // Incrementar la puntuación
                punt += 10;  // Puedes ajustar la cantidad de puntos según desees
            }
        }
    }
    // Update timer
    temps -= static_cast<int>(Rellotje.getElapsedTime().asSeconds());

    // Check if time is up
    if (temps <= 0) {
        std::cout << "Temps esgotat. Has perdut." << std::endl;
        return true;  // End the game
    }

    return resposta;
}

void crearmon(CircleShape& bola, RectangleShape& player) {
    bola.setFillColor(Color::Black);
    bola.setOrigin(bola.getRadius(), bola.getRadius());
    bola.setPosition(x_max / 2, y_max / 2);

    // Definim com és la barra del jugador
    player.setFillColor(Color::Black);
    player.setSize(Vector2f(longitut, amplada));
    FloatRect playerRect = player.getLocalBounds();
    player.setOrigin(playerRect.left + playerRect.width / 2.0f,
        playerRect.top + playerRect.height / 2.0f);
    player.setPosition(x_max / 2, y_max - 50);
}
bool ColisioAmbBlocs(sf::CircleShape& pilota, sf::RectangleShape& bloc) {
    // Busquem col·lisions entre la pilota i els blocs
    if (pilota.getGlobalBounds().intersects(bloc.getGlobalBounds())) {
        // Les formes estan col·lidint
        // Pots realitzar accions addicionals aquí si hi ha una col·lisió
        return true;  // Tornar true si hi ha col·lisió
    }
    else {
        // No hi ha col·lisió
        return false;  // Tornar false si no hi ha col·lisió
    }
}

void EntradaJugador(RenderWindow& finestre, RectangleShape& jugador, float dt) {
    sf::Event esdeveniment;
    while (finestre.pollEvent(esdeveniment)) {
        if (esdeveniment.type == sf::Event::Closed) {
            finestre.close();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && jugador.getPosition().x - longitut / 5 > x_min) {
        jugador.move(-playerSpeed, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && jugador.getPosition().x + longitut / 5 < x_max) {
        jugador.move(playerSpeed, 0);
    }
}

void DibuixarBlocs(RenderWindow& finestra, const std::vector<Bloque>& blocs) {
    for (const auto& bloc : blocs) {
        if (bloc.activo) {
            finestra.draw(bloc.shape);
        }
    }
}

void InicialitzarBlocs(std::vector<Bloque>& blocs) {
    blocs.clear();  // Netegem blocs anteriors (si hi ha)

    // Calcular la posición vertical inicial
    float startY = 50.0f;  // Ajusta la posición vertical según tus preferencias

    // Inicialitzem els blocs a la part superior de la pantalla
    for (int fila = 0; fila < numBloquesFilas; ++fila) {
        // Calcular la posición horizontal inicial para centrar los bloques
        float startX = (x_max - (numBloques * ampleBloc + (numBloques - 1) * espaiEntreBloques)) / 2.0f;

        for (int i = 0; i < numBloques; ++i) {
            Bloque bloque(sf::Vector2f(ampleBloc, alcadaBloc));
            bloque.shape.setFillColor(sf::Color::Red);
            bloque.shape.setPosition(startX + i * (ampleBloc + espaiEntreBloques), startY + fila * (alcadaBloc + espaiEntreBloques));
            blocs.push_back(bloque);
        }
    }
}



