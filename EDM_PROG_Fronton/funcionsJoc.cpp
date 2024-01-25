#include "funcionsJoc.h"

void Draw(RenderWindow& finestre, CircleShape& bola, RectangleShape& player, std::vector<Bloque>& blocs, int punt, int temps) {

    // Netegem la finestra amb un color Cyan
    finestre.clear(sf::Color::Cyan);

    // Dibuixem la pilota i el jugador
    finestre.draw(bola);
    finestre.draw(player);

    // Dibuixem els blocs
    DibuixarBlocs(finestre, blocs);

    // Carreguem la font per al text
    Font font;
    if (!font.loadFromFile("./fonts/KOMIKAP_.ttf")) {
        std::cerr << "No es va poder carregar la font" << std::endl;
        return;
    }

    // Creem i configurem el text de la puntuació
    Text puntuacioText;
    puntuacioText.setFont(font);
    puntuacioText.setCharacterSize(24);
    puntuacioText.setFillColor(sf::Color::Black);
    puntuacioText.setPosition(10, 10);
    puntuacioText.setString("Puntuació: " + std::to_string(punt));

    // Creem i configurem el text del temps
    Text tempsText;
    tempsText.setFont(font);
    tempsText.setCharacterSize(24);
    tempsText.setFillColor(sf::Color::Black);
    tempsText.setPosition(x_max - 170, 10);
    tempsText.setString("Temps: " + std::to_string(temps));

    // Dibuixem els textos a la finestra
    finestre.draw(puntuacioText);
    finestre.draw(tempsText);

    // Mostrem el contingut de la finestra
    finestre.display();
}

bool Update(float dt, CircleShape& bola, RectangleShape& player, float& ballSpeedX, float& ballSpeedY, std::vector<Bloque>& blocs, int& punt, int& temps, Clock Rellotje) {
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

    // Comprovem la col·lisió amb els blocs
    for (int fila = 0; fila < numBloquesFilas; ++fila) {
        for (int i = 0; i < numBloques; ++i) {
            int indice = fila * numBloques + i;
            if (blocs[indice].activo && ColisioAmbBlocs(bola, blocs[indice].shape)) {
                ballSpeedY = -ballSpeedY;
                blocs[indice].activo = false;

                // Incrementem la puntuació
                punt += 10;  // Pots ajustar la quantitat de punts segons desitgis
            }
        }
    }

    // Actualitzem el rellotge
    temps -= static_cast<int>(Rellotje.getElapsedTime().asSeconds());

    // Comprovem si s'ha acabat el temps
    if (temps <= 0) {
        std::cout << "Temps esgotat. Has perdut." << std::endl;
        return true;  // Finalitza el joc
    }

    return resposta;
}

void crearmon(CircleShape& bola, RectangleShape& player) {
    // Configurem la pilota
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
    // Processar els esdeveniments de la finestra
    sf::Event esdeveniment;
    while (finestre.pollEvent(esdeveniment)) {
        // Comprovar si es tanca la finestra
        if (esdeveniment.type == sf::Event::Closed) {
            finestre.close();
        }
    }

    // Moure el jugador a l'esquerra si s'apreta la tecla corresponent i no arriba al límit esquerre
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && jugador.getPosition().x - longitut / 5 > x_min) {
        jugador.move(-playerSpeed, 0);
    }

    // Moure el jugador a la dreta si s'apreta la tecla corresponent i no arriba al límit dret
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && jugador.getPosition().x + longitut / 5 < x_max) {
        jugador.move(playerSpeed, 0);
    }
}

void DibuixarBlocs(RenderWindow& finestra, const std::vector<Bloque>& blocs) {
    // Dibuixar els blocs actius a la finestra
    for (const auto& bloc : blocs) {
        if (bloc.activo) {
            finestra.draw(bloc.shape);
        }
    }
}

void InicialitzarBlocs(std::vector<Bloque>& blocs) {
    // Netegem els blocs existents (si n'hi ha)
    blocs.clear();

    // Calcular la posició vertical inicial
    float startY = 50.0f;  // Ajusta la posició vertical segons les teves preferències

    // Inicialitzem els blocs a la part superior de la pantalla
    for (int fila = 0; fila < numBloquesFilas; ++fila) {
        // Calcular la posició horitzontal inicial per centrar els blocs
        float startX = (x_max - (numBloques * ampleBloc + (numBloques - 1) * espaiEntreBloques)) / 2.0f;

        // Creem cada bloc i l'afegim a la llista
        for (int i = 0; i < numBloques; ++i) {
            Bloque bloque(sf::Vector2f(ampleBloc, alcadaBloc));
            bloque.shape.setFillColor(sf::Color::Red);
            bloque.shape.setPosition(startX + i * (ampleBloc + espaiEntreBloques), startY + fila * (alcadaBloc + espaiEntreBloques));
            blocs.push_back(bloque);
        }
    }
}