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
#include <optional>

using namespace std;

class RaceCarDriver{
private:
    Racer* car;

    // DFS state
    static stack<point> path;
    static set<pair<int,int>> visited;

    // Solution storage for run 2 & 3
    static vector<DIRECTION> solvedDirs;

    // Replay state for run 2 & 3
    static bool hasSolution;
    static bool replayMode;
    static size_t replayIndex;

    // Tracking
    static point lastPos;
    static DIRECTION lastDir;

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

        solvedDirs.clear();

        for (size_t i = rev.size(); i-- > 1; ) {
            solvedDirs.push_back(directionTo(rev[i], rev[i - 1]));
        }

        hasSolution = true;
        replayMode = true;
        replayIndex = 0;
    }

    void resetDfsState() {
        while (!path.empty()) path.pop();
        visited.clear();
    }

    // DFS Attempt
    bool pickDir(DIRECTION dir, point current, DIRECTION& outDir) {
        if (!car->look(dir)) {
            point next = nextPoint(current, dir);

            // Visited check ensures DFS hasn't already been there
            if (inBounds(next) && visited.count({next.x, next.y}) == 0) {
                path.push(next);
                visited.emplace(next.x, next.y);

                lastPos = current;
                lastDir = dir;
                outDir = dir;

                return true;
            }
        }
        return false;
}

public:
    RaceCarDriver(Racer* p = nullptr): car{p} {}

    DIRECTION nextMoveTeamThree() {
        if (car == nullptr) return EAST;

        point current = car->getLocation();

        // NEW RUN
        if (atStart(current) && !atStart(lastPos)) {
            if (replayMode) replayIndex = 0; // Happens on run 3 where we are in replaymode, but we need to reset replayIndex
            else {
                // Case 1: We found the solution in the last position, so we are saving the last path
                //         as our solution path for our next iteration.
                if (!replayMode && !path.empty() && !atStart(path.top())) {
                    saveCurrentPathAsSolution();
                }
                // Case 2: Either we failed in the iteration (hopefully doesn't happen), or we just iterated
                //         through the correct path
                else resetDfsState();
            }
        }

        // Run #2-3, we already have the solution, so we just iterate through it
        if (replayMode && replayIndex < solvedDirs.size()) {

            DIRECTION d = solvedDirs[replayIndex];
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

        DIRECTION dir;

        // 1. Try continuing forward first (no turn)
        if (pickDir(lastDir, current, dir)) {
            lastPos = current;
            return dir;
        }

        // 2. If not, then we look at all other directions
        for (DIRECTION d : DIRECTIONS) {
            if (d == lastDir) continue;

            DIRECTION out;
            if (pickDir(d, current, out)) {
                lastPos = current;
                return out;
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
vector<DIRECTION> RaceCarDriver::solvedDirs;
bool RaceCarDriver::replayMode = false;
bool RaceCarDriver::hasSolution = false;
size_t RaceCarDriver::replayIndex = 0;
point RaceCarDriver::lastPos = point(-1, -1);
DIRECTION RaceCarDriver::lastDir = EAST;

#endif /* RACECARDRIVER_H_ */
