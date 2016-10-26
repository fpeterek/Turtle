//
//  main.cpp
//  Turtle
//
//  Created by Filip Peterek on 04.08.16.
//  Copyright © 2016 Filip Peterek. All rights reserved.
//

#include <iostream>
#include <bitset>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "ResourcePath.hpp"
#include "App.hpp"

int main(int argc, const char * argv[]) {
    
    std::cout.sync_with_stdio(false);
    
    bool hasErrorOccured = false;
    std::string errorMessage;
    
    try {
        
        App app;
        app.Loop();
        
    }
    catch (std::exception & e) {
        
        hasErrorOccured = true;
        errorMessage = e.what();
        
    }
    catch (...) {
        
        hasErrorOccured = true;
        errorMessage = "An unexpected error has occured. ";
        
    }
    
    if (not hasErrorOccured) {
        return 0;
    }
    
    try {
        
        ErrorMessage message (errorMessage);
        message.Loop();
        
    } catch (std::exception & e) {
        
        std::endl(std::cout);
        std::cout << "An unexpected error has occured while displaying an error message. " << std::endl;
        std::cout << e.what() << std::endl;
        
    }
    
    return 0;
    
}
