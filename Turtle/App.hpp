//
//  App.hpp
//  Turtle
//
//  Created by Filip Peterek on 04.08.16.
//  Copyright © 2016 Filip Peterek. All rights reserved.
//

#ifndef App_hpp
#define App_hpp

#include <stdio.h>
#include <fstream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "Turtle.hpp"
#include "ConsoleInput.hpp"


class FileNotOpenException : public std::exception {
    
    std::string _filename;
    
public:
    
    FileNotOpenException(std::string filename);
    const char * what();
    
};

class ErrorMessage : sf::RenderWindow {
    
    std::string _message;
    sf::Text _text;
    sf::Font _font;
    sf::RectangleShape _background;
    sf::RectangleShape _button;
    sf::Text _buttonText;
    
    void Setup();
    void ButtonSetup();
public:
    
    ErrorMessage(std::string & param);
    ErrorMessage(const char * param);
    void Loop();
    
};

class App : sf::RenderWindow {
    
    Turtle _turtle;
    ConsoleInput _input;
    
    sf::Event _event;
    sf::RectangleShape _background;
    sf::RectangleShape _consoleLine;
    sf::RectangleShape _consoleBackground;
    sf::Font _consoleFont;
    sf::Text _consoleText;
    sf::Text _coordinates;
    
    static std::vector<std::string> Parse(std::string &);
    
    void ChangeBackground(std::vector<std::string> & param);
    void PollEvents();
    void UpdateConsole();
    void UpdateCoordinates();
    void Draw();
    
public:
    
    App();
    void Loop();
    
};


#endif /* App_hpp */
