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
    static vector<point> firstPath;
    static vector<point> secondPath;
    static vector<point> bestPath;
    static bool solutionReady;
    static size_t replayIndex;
    static point lastPos;
    static int attemptNumber;

    vector<DIRECTION> const DIRECTIONS = {EAST, SOUTH, WEST, NORTH};

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
        vector<point> rev;
        while (!temp.empty()) {
            rev.push_back(temp.top());
            temp.pop();
        }

        reverse(rev.begin(), rev.end());

        if (attemptNumber == 1){
            firstPath = rev;
            bestPath = firstPath;
        }
        else if (attemptNumber == 2){
            secondPath = rev;
            if (secondPath.size() < firstPath.size() && !secondPath.empty()){

            }
        }

        solutionReady = true;
        replayIndex = 1;
    }

    void resetDfsState() {
        while (!path.empty()) path.pop();
        visited.clear();
    }
    
    vector<DIRECTION> getDirections(){
        vector<DIRECTION> d = DIRECTIONS;
        if (attemptNumber == 2){
            reverse(d.begin(), d.end());
        }
        return d;
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
        if (attemptNumber >= 3 && solutionReady && replayIndex < bestPath.size()) {
            DIRECTION d = directionTo(current, bestPath[replayIndex]);
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
vector<point> RaceCarDriver::firstPath;
vector<point> RaceCarDriver::secondPath;
vector<point> RaceCarDriver::bestPath;
bool RaceCarDriver::solutionReady = false;
size_t RaceCarDriver::replayIndex = 0;
int RaceCarDriver::attemptNumber = 1;
point RaceCarDriver::lastPos = point(-1, -1);

#endif /* RACECARDRIVER_H_ */