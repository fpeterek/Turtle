//
//  ConsoleInput.cpp
//  Turtle
//
//  Created by Filip Peterek on 04.08.16.
//  Copyright © 2016 Filip Peterek. All rights reserved.
//

#include "ConsoleInput.hpp"


ConsoleInput::ConsoleInput() {
    
    _resetHistoryIter = true;
    
}

void ConsoleInput::ProcessEvent(sf::Event & event) {
    
    /* Unicode characters are not supported */
    
    if (event.text.unicode > 255) { return; }
    
    /* Console only accepts lower case letters, change upper case letters to lower case */
    
    if (event.text.unicode >= 65 and event.text.unicode <= 90) {
        
        event.text.unicode += 32;
        
    }
    
    /* If input is a lower case letter, space, hyphen or a number add it to variable _input */
    
    if (    (event.text.unicode >= 48 and event.text.unicode <= 57) or
            (event.text.unicode >= 97 and event.text.unicode <= 122) or
            event.text.unicode == 32 or event.text.unicode == 45
       ) {
     
        _input += event.text.unicode;
        
    }
    
    /* Delete last letter when backspace is pressed */
    
    if (event.key.code == 8) {
        
        if (_input.length()) {
            
            _input = _input.substr(0, _input.length() - 1);
            
        }
        
    }
    
}

void ConsoleInput::CommandHistory(sf::Event & event) {
    
    const long size = _inputHistory.size();
    static long i;
    static std::string oldInput = _input;
    
    if (not size) { return; }
    
    if (_resetHistoryIter) { i = size; _resetHistoryIter = false; }
    
    if (event.key.code == sf::Keyboard::Up) {
        
        if (i < 1) { return; }
        if (i == size) { oldInput = _input; }
        
        --i;
        _input = _inputHistory.at(i);
        
    }
    
    if (event.key.code == sf::Keyboard::Down) {
        
        if (i >= size) { return; }
        
        ++i;
        
        if (i == size) { _input = oldInput; }
        
        else { _input = _inputHistory.at(i); }
        
    }
    
}

std::string ConsoleInput::GetInput() {
    
    return _input;
    
}

void ConsoleInput::Clear() {
    
    if (_input.size()) { _inputHistory.emplace_back(_input); }
    _resetHistoryIter = true;
    _input = "";
    
}
