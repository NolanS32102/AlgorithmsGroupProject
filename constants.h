/*
* constants.h
 *
 *  Created on: Nov 12, 2025
 *      Author: bill_booth
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

struct Color{
    int r,g,b;
};

struct Parameter{
    set<pair<int,int> >* walls;
    SDL_Plotter* g;
};

//const int row = 40;
//const int col = 70;

const int row = 20;
const int col = 35;

const int block   = 22;
const int buffer  = 100;
const int padding = 6;
const int wallThickness = 3; // must be odd






#endif /* CONSTANTS_H_ */
