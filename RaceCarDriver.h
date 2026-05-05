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

    struct Point {
        int x;
        int y;

        Point(int x, int y) {
        this->x = x;
            this->y = y;
        }
        bool operator<(const Point& other) const {
            if (x != other.x) {
                return x < other.x;
            }
            return y < other.y;
        }
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };

    void addCurrentCellToGraph(map<Point, vector<Point>>& graph, Point currPos) {
        graph[currPos] = {};

        if (!car->look(EAST)) {
            graph[currPos].push_back(Point(currPos.x + 1, currPos.y));
        }

        if (!car->look(WEST)) {
            graph[currPos].push_back(Point(currPos.x - 1, currPos.y));
        }

        if (!car->look(NORTH)) {
            graph[currPos].push_back(Point(currPos.x, currPos.y - 1));
        }

        if (!car->look(SOUTH)) {
            graph[currPos].push_back(Point(currPos.x, currPos.y + 1));
        }
    }

    bool samePoint(const Point& a, const Point& b) {
        return a.x == b.x && a.y == b.y;
    }

    Point nextPoint(Point p, DIRECTION d) {
        if (d == EAST) p.x++;
        else if (d == SOUTH) p.y++;
        else if (d == WEST) p.x--;
        else p.y--;
        return p;
    }

    bool inBounds(const Point& p) {
        return p.x >= 0 && p.y >= 0 && p.x < col && p.y < row;
    }

    void resetDfsState(stack<Point>& path, set<Point>& visited) {
        while (!path.empty()) path.pop();
        visited.clear();
    }

    DIRECTION directionTo(Point from, Point to) {
        if (to.x > from.x) return EAST;
        if (to.x < from.x) return WEST;
        if (to.y > from.y) return SOUTH;
        return NORTH;
    }

    DIRECTION getDFSDir(
            stack<Point>& path,
            set<Point>& visited,
            Point& current,
            Point& endPt,
            Point& startPt,
            const int run) {
        vector<DIRECTION> const DIRECTIONS = {EAST, SOUTH, WEST, NORTH};

        // Decision
        for (DIRECTION dir: DIRECTIONS) {
            if (!car->look(dir)) {
                Point next = nextPoint(current, dir);
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
            Point child = current;
            path.pop();
            Point parent = path.top();
            current = parent;
            return directionTo(child, parent);    
        }
        return EAST;
    }

	DIRECTION nextMoveTeamThree(int run){
        // Pts
        static Point startingPoint = Point(0, 0);
        static Point endPoint = Point(-1, -1);
        static Point current = startingPoint;

        // DFS
        static stack<Point> path;
        static set<Point> visited;

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
                DIRECTION dir = getDFSDir(path, visited, current, endPoint, startingPoint, run);
                return dir;
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