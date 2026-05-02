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

enum State {
        DFS,
        REPLAY
    };

class RaceCarDriver{
private:

    Racer* car;
    static State mode;

    // DFS state
    static stack<point> path;
    static set<pair<int,int>> visited;

    // Solution storage for run 2 & 3
    static vector<DIRECTION> solvedDirs;

    // Replay state for run 2 & 3
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

        for (int i = (int)rev.size() - 1; i > 0; i--) {
            solvedDirs.push_back(
                directionTo(rev[i], rev[i - 1])
            );
        }

        cout << "solvedDirs output (size=" << solvedDirs.size() << ")" << endl;
        cout << "[";
        for (int i = 0; i < solvedDirs.size(); i++) {
            cout << solvedDirs[i] << ", ";
        }
        cout << "]" << endl;
    }

    void resetDfsState() {
        while (!path.empty()) path.pop();
        visited.clear();
        replayIndex = 0;
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
        point current = car->getLocation();

        if (mode == DFS && atStart(current) && !atStart(lastPos)) {
            if (path.size() > 1) {
                if (atStart(path.top())) {
                    path.pop();
                }
                saveCurrentPathAsSolution();
                if (!solvedDirs.empty()) {
                    mode = REPLAY;
                    replayIndex = 0;
                    lastPos = current;
                }
                else {
                    resetDfsState();
                    lastPos = current;
                }
            }
        }

        if (mode == REPLAY) {
            if (replayIndex >= solvedDirs.size()) {
                replayIndex = 0;
            }
            DIRECTION dir = solvedDirs[replayIndex++];
            lastPos = current;
            return dir;
        }

        // DFS
        if (path.empty()) {
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
State RaceCarDriver::mode = DFS;
size_t RaceCarDriver::replayIndex = 0;
point RaceCarDriver::lastPos = point(-1, -1);
DIRECTION RaceCarDriver::lastDir = EAST;

#endif /* RACECARDRIVER_H_ */
