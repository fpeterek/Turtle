//
//  Turtle.hpp
//  Turtle
//
//  Created by Filip Peterek on 04.08.16.
//  Copyright © 2016 Filip Peterek. All rights reserved.
//

#ifndef Turtle_hpp
#define Turtle_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <functional>

#include <SFML/Graphics.hpp>

#include "ResourcePath.hpp"

#define TURTLE_SPRITE       "sprite_1.png"
#define TURTLE_SPRITE_420   "sprite_2.png"


class MissingTextureException : public std::exception {
    
    std::string _textureName;
    
public:
    
    MissingTextureException(std::string filename);
    const char * what();
};

enum ActionList {
    
    Move,
    Rotate,
    Exit,
    ChangeColor
    
};

struct Action {
    
    std::string action;
    std::vector<int> parameters;
    bool isValidAction;
    bool isFinished;
    
    void SetAction(std::vector<std::string> &);
    void SetAction(Action &);
    
    Action();
    
};

class Turtle : public sf::Sprite {
    
    /* This map probably should have been a static member, but I couldn't compile it, idk why */
    /* Note: Turtle::InitMap was a static method, too, that wasn't the problem                */
    
    std::unordered_map<std::string, std::function<void(Turtle*)>> _methods;
    void InitMap();
    
    unsigned int _speed;
    unsigned int _thickness;
    bool _penDown;
    sf::Texture _texture;
    sf::Texture _texture420;
    sf::Color _color;
    std::vector<sf::RectangleShape> _lines;
    Action _action;
    
    void EasterEgg();
    void RotateTurtle();
    void MoveTurtle();
    void SetColor();
    void SetThickness();
    void SetSpeed();
    void Circle();
    void PenUp();
    void PenDown();
    void Jump();
    
    void DrawLine(unsigned int length);
    
public:
    
    Turtle();
    void ProcessCommand(std::vector<std::string> &);
    void Update();
    std::vector<sf::RectangleShape> & GetLines();
    
};

#endif /* Turtle_hpp */
