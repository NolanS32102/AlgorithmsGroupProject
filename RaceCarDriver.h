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
    stack<point> path;
    set<pair<int,int>> visited;

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
    RaceCarDriver(Racer* p = nullptr): car{p}{}

    DIRECTION nextMoveTeamThree() {

        point current = car->getLocation();

        if (path.empty()) {
            path.push(current);
            visited.emplace(current.x, current.y);
            return EAST;
        }

        if (!car->look(EAST)) {
            point next = nextPoint(current, EAST);
            if (visited.count({next.x, next.y}) == 0) {
                path.push(next);
                visited.emplace(next.x, next.y);
                return EAST;
            }
        }

        if (!car->look(SOUTH)) {
            point next = nextPoint(current, SOUTH);
            if (visited.count({next.x, next.y}) == 0) {
                path.push(next);
                visited.emplace(next.x, next.y);
                return SOUTH;
            }
        }

        if (!car->look(WEST)) {
            point next = nextPoint(current, WEST);
            if (visited.count({next.x, next.y}) == 0) {
                path.push(next);
                visited.emplace(next.x, next.y);
                return WEST;
            }
        }

        if (!car->look(NORTH)) {
            point next = nextPoint(current, NORTH);
            if (visited.count({next.x, next.y}) == 0) {
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
