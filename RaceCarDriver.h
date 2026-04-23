/*
* RaceCarDriver.h
 *
 *  Created on: Spring, 2026
 *      Author: bill_booth
 */

#ifndef RACECARDRIVER_H_
#define RACECARDRIVER_H_

#include "Racer.h"
#include <iostream>
#include <set>
#include <stack>
#include <vector>
using namespace std;

struct visitedPoint {
    point p;
    bool east, west, north, south;
};

class RaceCarDriver{
private:
    Racer* car;
    stack<point> path;
    set<pair<int,int>> visited;
    static const bool DEBUG_DFS = true;

    static vector<visitedPoint> maze;
    static int numRuns;

    bool samePoint(const point& a, const point& b) {
        return a.x == b.x && a.y == b.y;
    }

    bool safeByCurrentRules(const point& p) {
        return p.x >= 0 && p.y >= 0 && p.x < row && p.y < col;
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
    RaceCarDriver(Racer* p = nullptr): car{p}{
        numRuns += 1;
    }

    DIRECTION nextMoveTeamThree() {

        if (numRuns == 2) {
            
        }

        if (car == nullptr) return EAST;

        static int step = 0;
        static int maxX = 0;
        static int maxY = 0;
        step++;

        point current = car->getLocation();
        if (current.x > maxX) maxX = current.x;
        if (current.y > maxY) maxY = current.y;

        if (step % 100 == 1) {
            if (DEBUG_DFS) {
                cout << "[DFS] step=" << step
                     << " current=(" << current.x << "," << current.y << ")"
                     << " visited=" << visited.size()
                     << " stack=" << path.size()
                     << " maxReach=(" << maxX << "," << maxY << ")"
                     << endl;
            }
        }

        if (path.empty()) {
            path.push(current);
            visited.emplace(current.x, current.y);
            if (DEBUG_DFS) cout << "[DFS] init at (" << current.x << "," << current.y << ")" << endl;
        }

        if (!car->look(EAST)) {
            point next = nextPoint(current, EAST);
            if (!safeByCurrentRules(next)) {
                if (DEBUG_DFS) cout << "[DFS] skip EAST unsafe next=(" << next.x << "," << next.y << ")" << endl;
            }
            if (safeByCurrentRules(next) && visited.count({next.x, next.y}) == 0) {
                path.push(next);
                visited.emplace(next.x, next.y);
                return EAST;
            }
        }

        if (!car->look(SOUTH)) {
            point next = nextPoint(current, SOUTH);
            if (!safeByCurrentRules(next)) {
                if (DEBUG_DFS) cout << "[DFS] skip SOUTH unsafe next=(" << next.x << "," << next.y << ")" << endl;
            }
            if (safeByCurrentRules(next) && visited.count({next.x, next.y}) == 0) {
                path.push(next);
                visited.emplace(next.x, next.y);
                return SOUTH;
            }
        }

        if (!car->look(WEST)) {
            point next = nextPoint(current, WEST);
            if (!safeByCurrentRules(next)) {
                if (DEBUG_DFS) cout << "[DFS] skip WEST unsafe next=(" << next.x << "," << next.y << ")" << endl;
            }
            if (safeByCurrentRules(next) && visited.count({next.x, next.y}) == 0) {
                path.push(next);
                visited.emplace(next.x, next.y);
                return WEST;
            }
        }

        if (!car->look(NORTH)) {
            point next = nextPoint(current, NORTH);
            if (!safeByCurrentRules(next)) {
                if (DEBUG_DFS) cout << "[DFS] skip NORTH unsafe next=(" << next.x << "," << next.y << ")" << endl;
            }
            if (safeByCurrentRules(next) && visited.count({next.x, next.y}) == 0) {
                path.push(next);
                visited.emplace(next.x, next.y);
                return NORTH;
            }
        }

        if (path.size() > 1) {
            path.pop();
            point parent = path.top();
            if (DEBUG_DFS) {
                cout << "[DFS] dead-end backtrack from (" << current.x << "," << current.y
                     << ") to (" << parent.x << "," << parent.y << ")"
                     << " visited=" << visited.size() << " stack=" << path.size() << endl;
            }
            return directionTo(current, parent);
        }

        if (DEBUG_DFS) {
            cout << "[DFS] root stuck at (" << current.x << "," << current.y
                 << ") visited=" << visited.size()
                 << " maxReach=(" << maxX << "," << maxY << ")" << endl;
        }
        return EAST;
    }
};

#endif /* RACECARDRIVER_H_ */
