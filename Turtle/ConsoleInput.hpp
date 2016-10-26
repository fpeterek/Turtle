//
//  ConsoleInput.hpp
//  Turtle
//
//  Created by Filip Peterek on 04.08.16.
//  Copyright © 2016 Filip Peterek. All rights reserved.
//

#ifndef ConsoleInput_hpp
#define ConsoleInput_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>


class ConsoleInput {
    
    std::string _input;
    std::vector<std::string> _inputHistory;
    bool _resetHistoryIter;
    
public:
    
    std::string GetInput();
    void ProcessEvent(sf::Event &);
    void CommandHistory(sf::Event &);
    void Clear();
    ConsoleInput();
    
};

#endif /* ConsoleInput_hpp */
