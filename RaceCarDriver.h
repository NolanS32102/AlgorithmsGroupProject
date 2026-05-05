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

public:
	RaceCarDriver(Racer* p = nullptr): car{p}{}

    class TeamThreePoint{
        public:
            TeamThreePoint(int x, int y): x(x), y(y){}
            TeamThreePoint(): x(-1), y(-1){}
            int x, y;

            bool operator<(const TeamThreePoint& other) const {
                if (x != other.x) return x < other.x;
                return y < other.y;
            }
    };

    bool samePoint(const TeamThreePoint& a, const TeamThreePoint& b) {
        return a.x == b.x && a.y == b.y;
    }

    TeamThreePoint nextPoint(TeamThreePoint p, DIRECTION d) {
        if (d == EAST) p.x++;
        else if (d == SOUTH) p.y++;
        else if (d == WEST) p.x--;
        else p.y--;
        return p;
    }

    bool inBounds(const TeamThreePoint& p) {
        return p.x >= 0 && p.y >= 0 && p.x < col && p.y < row;
    }

    void resetDfsState(stack<TeamThreePoint>& path, set<TeamThreePoint>& visited) {
        while (!path.empty()) path.pop();
        visited.clear();
    }

    DIRECTION directionTo(TeamThreePoint from, TeamThreePoint to) {
        if (to.x > from.x) return EAST;
        if (to.x < from.x) return WEST;
        if (to.y > from.y) return SOUTH;
        return NORTH;
    }

    DIRECTION getDFSDir(
            stack<TeamThreePoint>& path,
            set<TeamThreePoint>& visited,
            TeamThreePoint& current,
            TeamThreePoint& endPt,
            TeamThreePoint& startPt,
            const int run) {
        vector<DIRECTION> const DIRECTIONS = {EAST, SOUTH, WEST, NORTH};

        // Decision
        for (DIRECTION dir: DIRECTIONS) {
            if (!car->look(dir)) {
                TeamThreePoint next = nextPoint(current, dir);
                if (inBounds(next) 
                        && visited.count(next) == 0 
                        && (!samePoint(next, endPt))) {
                    path.push(next);
                    visited.emplace(next);
                    current = next;
                    return dir;
                }
            }
        }

        // Backtracking
        if (path.size() > 1) {
            TeamThreePoint child = current;
            path.pop();
            TeamThreePoint parent = path.top();
            current = parent;
            return directionTo(child, parent);    
        }
        return EAST;
    }

	DIRECTION nextMoveTeamThree(int run){
        // Pts
        static TeamThreePoint startingPoint = TeamThreePoint(0, 0);
        static TeamThreePoint endPoint = TeamThreePoint();
        static TeamThreePoint current = startingPoint;

        // DFS
        static stack<TeamThreePoint> path;
        static set<TeamThreePoint> visited;

        // Dijkstra's stuff goes here (wtf is dijkstra??)

        // Best path
        static int lastRun = -1;
        static vector<DIRECTION> bestPath;
        static size_t bestPathIdx = 0;

        if (run != lastRun) {
            // Preserve the finish location discovered during run 0.
            // At the moment run changes from 0 -> 1, 'current' is still
            // the logical point reached in run 0 (the finish tile).
            if (lastRun == 0 && run == 1) {
                endPoint = current;
            }

            current = startingPoint;
            resetDfsState(path, visited);
            path.push(startingPoint);
            visited.emplace(startingPoint);

            if (run == 2) bestPathIdx = 0;
            lastRun = run;
        }

		switch (run) {
            // RUN DFS to only find the finish
            case 0: {
                DIRECTION dir = getDFSDir(path, visited, current, endPoint, startingPoint, run);
                return dir;
                break;
            }

            // RUN DFS to create the topology (do not go to the finish)
            case 1: {
                if (run == 1 && samePoint(current, startingPoint)) {
                    // "Flag on" DIJKSTRAS & begin running it here
                }
                DIRECTION dire = getDFSDir(path, visited, current, endPoint, startingPoint, run);
                return dire;
                break;
            }

            // Traverse the vector of directions
            case 2: {
                break;
            }

            default: {
                break;
            }
        }

        // Fallback to satisfy all control paths.
        return NORTH;
    }
};


#endif /* RACECARDRIVER_H_ */