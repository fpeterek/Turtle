//
//  App.cpp
//  Turtle
//
//  Created by Filip Peterek on 04.08.16.
//  Copyright © 2016 Filip Peterek. All rights reserved.
//

#include "App.hpp"


/* Exception, currently serves no purpose */

FileNotOpenException::FileNotOpenException(std::string filename) {
    
    _filename = filename;
    
}

const char * FileNotOpenException::what() {
    
    std::string s = "File" + _filename + "could not be opened. ";
    return s.c_str();
    
}

/* Error message */

void ErrorMessage::ButtonSetup() {
    
    _buttonText.setFont(_font);
    _buttonText.setString("Exit");
    _buttonText.setColor(sf::Color(60, 60, 60));
    /* Stupid way of doing it, but the string never changes, so it should work, it just isn't very portable */
    _buttonText.setPosition(getSize().x / 2 - 29, getSize().y / 2 + 3);
    
    _button.setFillColor(sf::Color(230, 230, 230));
    _button.setSize(sf::Vector2f(100, 50));
    _button.setOutlineColor(sf::Color(180, 180, 180));
    _button.setOutlineThickness(4);
    
    _button.setPosition(getSize().x / 2 - 50, getSize().y / 2);
    
}

void ErrorMessage::Setup() {
    
    _font.loadFromFile(resourcePath() + "Inconsolata-Regular.ttf");
    _text.setFont(_font);
    
    _text.setColor(sf::Color::Black);
    _text.setString(_message);
    _text.setCharacterSize(30);
    
    sf::VideoMode temp;
    temp.height = _text.getGlobalBounds().height + 150;
    temp.width = _text.getGlobalBounds().width + 50;
    
    _text.setPosition(25, 0);
    
    _background.setSize( sf::Vector2f(temp.height, temp.width) );
    _background.setFillColor(sf::Color::White);
    _background.setSize(sf::Vector2f(2000, 2000));
    create(temp, "Error");
    setFramerateLimit(60);
    
    ButtonSetup();
    
}

ErrorMessage::ErrorMessage(std::string & param) {
    
    _message = param;
    Setup();
    
}

ErrorMessage::ErrorMessage(const char * param) {
    
    _message = param;
    Setup();
    
}

void ErrorMessage::Loop() {
    
    sf::Event event;
    
    while (isOpen()) {
        
        while(pollEvent(event)) {
            
            if (event.type == sf::Event::Closed) close();
            if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Return) close();
            if (event.type == sf::Event::MouseButtonPressed and
                _button.getGlobalBounds().contains(sf::Mouse::getPosition(*this).x, sf::Mouse::getPosition(*this).y)) close();
            
        }
        
        clear();
        draw(_background);
        draw(_text);
        draw(_button);
        draw(_buttonText);
        display();
        
    }
    
}

/* App */

App::App() {
    
    create(sf::VideoMode::getFullscreenModes().at(2), "Turtle");
    setFramerateLimit(60);
    
    _consoleLine.setFillColor(sf::Color(126, 130, 154));
    _consoleBackground.setFillColor(sf::Color(33, 36, 50));
    _consoleFont.loadFromFile(resourcePath() + "Inconsolata-Regular.ttf");
    _consoleText.setFont(_consoleFont);
    
    _coordinates.setFont(_consoleFont);
    _coordinates.setCharacterSize(40);
    // _coordinates.setPosition(getSize().x - 350, 20);
    _coordinates.setColor(sf::Color::Black);
    
    _background.setFillColor(sf::Color::White);
    
}

void App::Draw() {
    
    clear();
    
    draw(_background);
    for (auto & line : _turtle.GetLines()) {
        
        draw(line);
        
    }
    draw(_turtle);
    
    draw(_consoleBackground);
    draw(_consoleLine);
    draw(_consoleText);
    draw(_coordinates);
    
    display();
    
}

void App::UpdateCoordinates() {
    
    std::stringstream ss;
    ss << "x: " << _turtle.getPosition().x << " y: " << _turtle.getPosition().y;
    _coordinates.setString(ss.str());
    _coordinates.setPosition(getSize().x - _coordinates.getLocalBounds().width - 20, 5);
    
}

void App::UpdateConsole() {
    
    /* Updates console - Updates the text and resizes and moves the console if the size of the window changes */
    
    _consoleLine.setSize(sf::Vector2f(getSize().x, 5));
    _consoleLine.setPosition(0, getSize().y - 45);
    
    _consoleBackground.setSize(sf::Vector2f(getSize().x, 40));
    _consoleBackground.setPosition(0, getSize().y - 40);
    
    _consoleText.setPosition(0, getSize().y - 35);
    _consoleText.setString(_input.GetInput());
    
}

void App::ChangeBackground(std::vector<std::string> & params) {
    
    uint8_t r, g, b;
    
    try {
        
        r = std::stoi(params[1]);
        g = std::stoi(params[2]);
        b = std::stoi(params[3]);
        
    } catch (std::out_of_range & e) {
        
        return;
        
    } catch (std::invalid_argument & e) {
        
        return;
        
    }
    
    _background.setFillColor(sf::Color(r, g, b));
    
    /* Change text color to the opposite color of the background color                           */
    /* Unless the background is grey, because ~128 is 127 and those colors are nearly identical  */
    /* If background is grey (r, g, b == 128 ± 18), set color to black and just flip it to white */
    
    if (r >= 110 and r <= 146 and g >= 110 and g <= 146 and b >= 110 and b <= 146) {
        
        r = 0;
        b = 0;
        g = 0;
        
    }
    
    r = ~r;
    g = ~g;
    b = ~b;
    
    _coordinates.setColor(sf::Color(r, g, b));
    
}

void App::PollEvents() {
    
    std::string input;
    std::vector<std::string> tokens;
    
    while (pollEvent(_event)) {
        
        if (_event.type == sf::Event::KeyPressed and _event.key.code == sf::Keyboard::Return) {
            
            input = _input.GetInput();
            
            tokens = App::Parse(input);
            
            if (tokens[0] == "exit") { close(); }
            
            if (tokens[0] == "background") { ChangeBackground(tokens); }
            else { _turtle.ProcessCommand(tokens); }
            
            _input.Clear();
            
        }
        
        if (_event.type == sf::Event::KeyPressed and (_event.key.code == sf::Keyboard::Up or _event.key.code == sf::Keyboard::Down)) {
            
            _input.CommandHistory(_event);
            
        }
        
        if (_event.type == sf::Event::TextEntered) {
            
            _input.ProcessEvent(_event);
            
        }
        
        else if (_event.type == sf::Event::Closed) {
            
            close();
            
        }
        
    }
    
}

void App::Loop() {
    
    while (isOpen()) {
        
        PollEvents();
        
        /* Change background size if window size changes */
        _background.setSize(sf::Vector2f(getSize()));
        
        UpdateConsole();
        _turtle.Update();
        UpdateCoordinates();
        
        Draw();
        
    }
    
    return;
    
}


std::vector<std::string> App::Parse(std::string & input) {
    
    std::vector<std::string> tokens;
    
    std::string token;
    
    long position;
    
    while ((position = input.find(" ")) != std::string::npos) {
        
        token = input.substr(0, position);
        
        if (token.find_first_not_of(" ") != std::string::npos) {
            
            tokens.emplace_back(token);
            
        }
        
        input.erase(0, position + 1);
        
    }
    
    tokens.emplace_back(input);
    return tokens;
    
}

/*                                                                */
/*                                                                */
/*                                                                */
/*                                                                */
/*----------------------------------------------------------------*/
/*                                                                */
/*                                                                */
/*                      Let's build a ladder                      */
/*                                                                */
/*                                                                */
/*----------------------------------------------------------------*/
/*                                                                */
/*                                                                */
/*                      Let's build a ladder                      */
/*                                                                */
/*                                                                */
/*----------------------------------------------------------------*/
/*                                                                */
/*                                                                */
/*                      Let's build a ladder                      */
/*                                                                */
/*                                                                */
/*----------------------------------------------------------------*/
/*                                                                */
/*                                                                */
/*                      Let's build a ladder                      */
/*                                                                */
/*                                                                */
/*----------------------------------------------------------------*/
/*                                                                */
/*                                                                */
/*                      Let's build a ladder                      */
/*                                                                */
/*                                                                */
/*----------------------------------------------------------------*/
/*                                                                */
/*                                                                */
/*                      Let's build a ladder                      */
/*                                                                */
/*                                                                */
/*----------------------------------------------------------------*/
/*                                                                */
/*                                                                */
/*                      Let's build a ladder                      */
/*                                                                */
/*                                                                */
/*----------------------------------------------------------------*/
/*                                                                */
/*                                                                */
/*                      Let's build a ladder                      */
/*                                                                */
/*                                                                */
/*----------------------------------------------------------------*/
/*                                                                */
/*                                                                */
/*                      Let's build a ladder                      */
/*                                                                */
/*                                                                */
/*----------------------------------------------------------------*/
/*                                                                */
/*                                                                */
/*                                                                */
/*                                                                */
