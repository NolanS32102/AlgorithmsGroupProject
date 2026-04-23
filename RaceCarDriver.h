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
using namespace std;

class RaceCarDriver{
private:
    Racer* car;
    stack<point> path;
    set<pair<int,int>> visited;

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

public:
    RaceCarDriver(Racer* p = nullptr): car{p} {}

    DIRECTION nextMoveTeamThree() {

        point current = car->getLocation();

        if (current.x == 0 && current.y == 0 && !path.empty() && !samePoint(path.top(), current)) {
            while (!path.empty()) path.pop();
            visited.clear();
        }

        if (path.empty()) {
            path.push(current);
            visited.emplace(current.x, current.y);
        }

        if (!car->look(EAST)) {
            point next = nextPoint(current, EAST);
            if (inBounds(next) && visited.count({next.x, next.y}) == 0) {
                path.push(next);
                visited.emplace(next.x, next.y);
                return EAST;
            }
        }

        if (!car->look(SOUTH)) {
            point next = nextPoint(current, SOUTH);
            if (inBounds(next) && visited.count({next.x, next.y}) == 0) {
                path.push(next);
                visited.emplace(next.x, next.y);
                return SOUTH;
            }
        }

        if (!car->look(WEST)) {
            point next = nextPoint(current, WEST);
            if (inBounds(next) && visited.count({next.x, next.y}) == 0) {
                path.push(next);
                visited.emplace(next.x, next.y);
                return WEST;
            }
        }

        if (!car->look(NORTH)) {
            point next = nextPoint(current, NORTH);
            if (inBounds(next) && visited.count({next.x, next.y}) == 0) {
                path.push(next);
                visited.emplace(next.x, next.y);
                return NORTH;
            }
        }

        if (path.size() > 1) {
            path.pop();
            point parent = path.top();
            return directionTo(current, parent);
        }
        
        return EAST;
    }
};

#endif /* RACECARDRIVER_H_ */
