/*
* Author: Karter Sanamo, Christine Seng, Georgia Rushing,
* Matthew Peterson, Nolan Schirripa
* Assignment Title: Group Project
* Assignment Description: driver for nextMoveTeamThree
* function that solves the maze
* Due Date: 5/03/2026
* Date Created: 4/23/2026
* Date Last Modified: 5/03/2026
*/


/*
* RaceCarDriver.h
 *
 *  Created on: Spring, 2026
 *      Author: bill_booth
 */

#ifndef RACECARDRIVER_H_
#define RACECARDRIVER_H_

#include "Racer.h"
#include <set>
#include <stack>
#include <vector>
#include <algorithm>
using namespace std;

class RaceCarDriver{
private:
    Racer* car;
    static stack<point> path;
    static set<pair<int,int>> visited;
    static vector<DIRECTION> firstDirs;
    static vector<DIRECTION> secondDirs;
    static vector<DIRECTION>* bestDirs;
    static bool solutionReady;
    static size_t replayIndex;
    static point lastPos;
    static int attemptNumber;

    vector<DIRECTION> const DIRECTIONS_ORIGINAL = {EAST, SOUTH, WEST, NORTH};
    vector<DIRECTION> const DIRECTIONS_REVERSED = {SOUTH, WEST, NORTH, EAST};

    bool samePoint(const point& a, const point& b) {
        return a.x == b.x && a.y == b.y;
    }

    bool inBounds(const point& p) {
        return p.x >= 0 && p.y >= 0 && p.x < col && p.y < row;
    }

    DIRECTION directionTo(point from, point to) {
        if (to.x > from.x) return EAST;
        if (to.x < from.x) return WEST;
        if (to.y > from.y) return SOUTH;
        return NORTH;
    }

    point nextPoint(point p, DIRECTION d) {
        if (d == EAST) p.x++;
        else if (d == SOUTH) p.y++;
        else if (d == WEST) p.x--;
        else p.y--;
        return p;
    }

    bool atStart(const point& p) {
        return p.x == 0 && p.y == 0;
    }

    void saveCurrentPathAsSolution() {
        stack<point> temp = path;
        vector<DIRECTION> rev;
        while (!temp.empty()) {
            point parent = temp.top();
            temp.pop();

            if (!temp.empty()) {
                rev.push_back(directionTo(temp.top(), parent));
            }
        }

        reverse(rev.begin(), rev.end());

        if (attemptNumber == 1){
            firstDirs = rev;
            bestDirs = &firstDirs;
        }
        else if (attemptNumber == 2){
            secondDirs = rev;
            if (secondDirs.size() < firstDirs.size() && !secondDirs.empty()){
                bestDirs = &secondDirs;
            }
        }

        solutionReady = true;
        replayIndex = 0;
    }

    void resetDfsState() {
        while (!path.empty()) path.pop();
        visited.clear();
    }
    
    vector<DIRECTION> getDirections(){
        // run second attempt with different set of directions
        if (attemptNumber == 2){
            return DIRECTIONS_REVERSED;
        }
        return DIRECTIONS_ORIGINAL;
    }

public:
    RaceCarDriver(Racer* p = nullptr): car{p} {}

    DIRECTION nextMoveTeamThree() {
        if (car == nullptr) return EAST;

        point current = car->getLocation();

        // NEW RUN
        if (atStart(current) && !atStart(lastPos) && lastPos.x != -1) {
            if (!path.empty()){
                saveCurrentPathAsSolution();
            }
            resetDfsState();
            attemptNumber++;
        }

        // Run #3, we already have the solution, so we just iterate through it
        if (bestDirs && attemptNumber >= 3 && solutionReady && replayIndex < bestDirs->size()) {
            DIRECTION d = (*bestDirs)[replayIndex];
            replayIndex++;
            lastPos = current;
            return d;
        }

        // Run #1-2:
        if (path.empty()) {
            // First cell
            path.push(current);
            visited.emplace(current.x, current.y);
        }

        // Check each direction, traverse if it's available
        for (DIRECTION dir: getDirections()) {
            if (!car->look(dir)) {
                point next = nextPoint(current, dir);
                // Checks in bounds & not already visited
                if (inBounds(next) && visited.count({next.x, next.y}) == 0) {
                    path.push(next);
                    visited.emplace(next.x, next.y);
                    lastPos = current;
                    return dir;
                }
            }
        }

        // Backtracking case
        if (path.size() > 1) {
            path.pop();
            point parent = path.top();
            lastPos = current;
            return directionTo(current, parent);
        }

        // We failed ;(
        lastPos = current;
        car->die();
        return EAST;
    }
};

stack<point> RaceCarDriver::path;
set<pair<int,int>> RaceCarDriver::visited;
vector<DIRECTION> RaceCarDriver::firstDirs;
vector<DIRECTION> RaceCarDriver::secondDirs;
vector<DIRECTION>* RaceCarDriver::bestDirs = nullptr;
bool RaceCarDriver::solutionReady = false;
size_t RaceCarDriver::replayIndex = 0;
int RaceCarDriver::attemptNumber = 1;
point RaceCarDriver::lastPos = point(-1, -1);

#endif /* RACECARDRIVER_H_ */