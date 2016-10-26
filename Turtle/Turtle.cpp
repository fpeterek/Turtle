//
//  Turtle.cpp
//  Turtle
//
//  Created by Filip Peterek on 04.08.16.
//  Copyright © 2016 Filip Peterek. All rights reserved.
//

#include "Turtle.hpp"


MissingTextureException::MissingTextureException(std::string filename) {
    
    _textureName = filename;
    
}

const char * MissingTextureException::what() {
    
    std::string s = "Missing Texture: " + _textureName;
    return s.c_str();
    
}

/************************************************************************************************************************/
/*                                                                                                                      */
/*   --------------------------------------------------   Action   --------------------------------------------------   */
/*                                                                                                                      */
/************************************************************************************************************************/

Action::Action() { isValidAction = true; isFinished = true; }

void Action::SetAction(std::vector<std::string> & params) {
    
    if (not params.size()) { isValidAction = false; return; }
    
    action = params[0];
    parameters.clear();
    for (int i = 1; i < params.size(); ++i) {
        
        if (params[i] == "pi") { parameters.emplace_back(180); continue; }
        else if (params[i] == "-pi") { parameters.emplace_back(-180); continue; }
        
        try {
            
            parameters.emplace_back(std::stoi(params[i]));
            
        } catch (const std::invalid_argument & e) {
            
            continue;
            
        }
        
    }
    
    isFinished = false;
    isValidAction = true;
    
}

void Action::SetAction(Action & action) {
    
    *this = action;
    isFinished = false;
    
}


/************************************************************************************************************************/
/*                                                                                                                      */
/*   --------------------------------------------------   Turtle   --------------------------------------------------   */
/*                                                                                                                      */
/************************************************************************************************************************/

/* Initializes map of method pointers */

void Turtle::InitMap() {
    
    Turtle::_methods.emplace("420",         &Turtle::EasterEgg      );
    Turtle::_methods.emplace("rotate",      &Turtle::RotateTurtle   );
    Turtle::_methods.emplace("move",        &Turtle::MoveTurtle     );
    Turtle::_methods.emplace("circle",      &Turtle::Circle         );
    Turtle::_methods.emplace("color",       &Turtle::SetColor       );
    Turtle::_methods.emplace("thickness",   &Turtle::SetThickness   );
    Turtle::_methods.emplace("speed",       &Turtle::SetSpeed       );
    Turtle::_methods.emplace("penup",       &Turtle::PenUp          );
    Turtle::_methods.emplace("pendown",     &Turtle::PenDown        );
    Turtle::_methods.emplace("jump",        &Turtle::Jump           );

}

Turtle::Turtle() {
    
    _speed = 1.f;
    if ( not _texture.loadFromFile(resourcePath() + TURTLE_SPRITE) ) {
        
        throw MissingTextureException(TURTLE_SPRITE);
        
    }
    if (not _texture420.loadFromFile(resourcePath() + TURTLE_SPRITE_420) ) {
        
        throw MissingTextureException(TURTLE_SPRITE_420);
        
    }
    _color = sf::Color::Black;
    _thickness = 2;
    _penDown = false;
    
    rotate(90);
    setTexture(_texture);
    setScale(2.f, 2.f);
    setPosition(getTextureRect().width, getTextureRect().height);
    setOrigin(getTextureRect().width / 2.f, getTextureRect().height / 2.f);
    
    InitMap();
    
}

void Turtle::ProcessCommand(std::vector<std::string> & arg) {
    
    if (not _action.isFinished) { return; }
    
    _action.SetAction(arg);
    
}

std::vector<sf::RectangleShape> & Turtle::GetLines() {
    
    std::vector<sf::RectangleShape> & ref = _lines;
    return ref;
    
}

void Turtle::Update() {
    
    if (_action.isFinished or not _action.isValidAction) { return; }
    
    /* Call the method or declare action invalid */
    /* std::unordered_map::at() throws if key isn't found */
    
    try {
        
        /* Call member function and pass this as a parameter */
        Turtle::_methods.at(_action.action)(this);
        
    } catch (const std::out_of_range & e) {
        
        _action.isValidAction = false;
        _action.isFinished = true;
        
    }
    
}

void Turtle::EasterEgg() {
    
    setTexture(_texture420);
    _action.isFinished = true;
    
}

void Turtle::RotateTurtle() {
    
    if (_action.parameters[0] == 0) { _action.isFinished = true; return; }
    
    bool negative = _action.parameters[0] < 0;
    
    int rotateBy = negative ? (5 * _speed * -1) : (5 * _speed);
    if (not negative and rotateBy > _action.parameters[0])  { rotateBy = _action.parameters[0]; }
    else if (negative and rotateBy < _action.parameters[0]) { rotateBy = _action.parameters[0]; }
    
    rotate(rotateBy);
    
    _action.parameters[0] -= rotateBy;
    
}

void Turtle::MoveTurtle() {
    
    if (_action.parameters[0] <= 0) { _action.isFinished = true; return; }
    
    /* Rotation is 0 when turtle is pointing upwards, when it actually should be 270. Subtracting 90 fixes this */
    float rotation = getRotation() - 90;
    float offsetX, offsetY;
    unsigned int moveBy = 5 * _speed;
    
    if (moveBy > _action.parameters[0]) { moveBy = _action.parameters[0]; }
    
    offsetX = cos(rotation * M_PI / 180.0) * moveBy;
    offsetY = sin(rotation * M_PI / 180.0) * moveBy;
    
    DrawLine(moveBy);
    
    move(offsetX, offsetY);
    
    _action.parameters[0] -= moveBy;
    
}

void Turtle::Circle() {
    
    if (_action.parameters.size() == 1) { _action.parameters.push_back(360); }
    
    if (_action.parameters[1] == 0) {
        
        _action.isFinished = true;
        
    }
    int rotateBy, moveBy, offsetX, offsetY;
    
    const int radius   = _action.parameters[0];
    const int rotation = getRotation() - 90;
    
    if (_action.parameters[1] >= 10) {
    
        rotateBy = 10;
        
    }
    else if (_action.parameters[1] <= -10) {
        
        rotateBy = -10;
        
    }
    else {
        
        rotateBy = _action.parameters[1];
        
    }
    
    moveBy = round(sin(10 * M_PI / 180.0) * radius);
    offsetX = round(cos(rotation * M_PI / 180.0) * moveBy);
    offsetY = round(sin(rotation * M_PI / 180.0) * moveBy);
    
    DrawLine(moveBy);
    
    rotate(rotateBy);
    move(offsetX, offsetY);
    
    _action.parameters[1] -= rotateBy;
    
    return;
    
}

void Turtle::SetColor() {
    
    for (unsigned char i = 0; i < 3; ++i) {
    
        if (_action.parameters[i] < 0 or _action.parameters[i] > 255) {
        
            _action.isValidAction = false;
            _action.isFinished = true;
            return;
        
        }
        
    }
    
    _color.r = _action.parameters[0];
    _color.g = _action.parameters[1];
    _color.b = _action.parameters[2];
    _action.isFinished = true;
    
}

void Turtle::SetThickness() {
    
    if (_action.parameters[0] < 0) {
    
        _action.isValidAction = false;
        _action.isFinished = true;
        return;
    
    }
    
    _thickness = _action.parameters[0];
    _action.isFinished = true;
    return;
    
}

void Turtle::SetSpeed() {
    
    if (_action.parameters[0] <= 0) {
        
        _action.isValidAction = false;
        _action.isFinished = true;
        return;
        
    }
    
    _speed = _action.parameters[0];
    _action.isFinished = true;
    return;

}

void Turtle::PenUp() {
    
    _penDown = false;
    _action.isFinished = true;
    
}

void Turtle::PenDown() {
    
    _penDown = true;
    _action.isFinished = true;
    
}

void Turtle::Jump() {
    
    if (_action.parameters.size() < 2) {
        
        _action.isValidAction = false;
        
    } else {
    
        setPosition(_action.parameters[0], _action.parameters[1]);
        
    }
    
    _action.isFinished = true;
    
}

void Turtle::DrawLine(unsigned int length) {
    
    /* Probably the most memory heavy way of doing this, but memory is cheap so this has virtually no impact,   */
    /* unless you're trying to draw Mona Lisa or something                                                      */
    
    if (_penDown) {
        sf::RectangleShape rect(sf::Vector2f(length, _thickness));
        rect.setOrigin(0, round(_thickness / 2.0));
        rect.setRotation(getRotation() - 90);
        rect.setFillColor(_color);
        rect.setPosition(getPosition());
        _lines.push_back(rect);
    }

}
