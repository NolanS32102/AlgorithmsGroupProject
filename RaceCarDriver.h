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
using namespace std;

class RaceCarDriver{
private:
    Racer* car;
    static stack<point> path;
    static set<pair<int,int>> visited;
    static vector<point> solvedPath;
    static bool solutionReady;
    static size_t replayIndex;
    static point lastPos;

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

        solvedPath.clear();
        for (int i = (int)rev.size() - 1; i >= 0; --i) {
            solvedPath.push_back(rev[i]);
        }

        solutionReady = true;
        replayIndex = 1;
    }

    void resetDfsState() {
        while (!path.empty()) path.pop();
        visited.clear();
    }

public:
    RaceCarDriver(Racer* p = nullptr): car{p} {}

    DIRECTION nextMoveTeamThree() {
        if (car == nullptr) return EAST;

        point current = car->getLocation();

        // NEW RUN
        if (atStart(current) && !atStart(lastPos)) {
            // Case 1: We found the solution in the last position, so we are saving the last path
            //         as our solution path for our next iteration.
            if (!solutionReady && !path.empty() && !atStart(path.top())) saveCurrentPathAsSolution();
            // Case 2: Either we failed in the iteration (hopefully doesn't happen), or we just iterated
            //         through the correct path
            else resetDfsState();
        }

        // Run #2-3, we already have the solution, so we just iterate through it
        if (solutionReady && replayIndex < solvedPath.size()) {
            DIRECTION d = directionTo(current, solvedPath[replayIndex]);
            replayIndex++;
            lastPos = current;
            return d;
        }

        // Run #1:
        if (path.empty()) {
            // First cell
            path.push(current);
            visited.emplace(current.x, current.y);
        }

        // Check each direction, traverse if its available
        for (DIRECTION dir: DIRECTIONS) {
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
vector<point> RaceCarDriver::solvedPath;
bool RaceCarDriver::solutionReady = false;
size_t RaceCarDriver::replayIndex = 0;
point RaceCarDriver::lastPos = point(-1, -1);

#endif /* RACECARDRIVER_H_ */
