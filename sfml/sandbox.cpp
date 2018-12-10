#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bla bla bla bla bla");
    sf::CircleShape shape(100.f);
    sf::View viewport = window.getView();
    
    shape.setFillColor(sf::Color::Red);
    
    sf::Font ubMono;
    ubMono.loadFromFile("./ubuntu-font/UbuntuMono-B.ttf");
    sf::Text text("", ubMono, 12);
    
    text.setString("Hello World!");
    text.setColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    text.setPosition(viewport.getCenter().x, viewport.getCenter().y);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        shape.setPosition(viewport.getCenter().x, viewport.getCenter().y);
        text.setPosition(viewport.getCenter().x, viewport.getCenter().y);
        window.clear();
        window.draw(shape);
        window.draw(text);
        window.display();
    }

    return 0;
}