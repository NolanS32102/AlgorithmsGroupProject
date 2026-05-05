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
#include <map>
using namespace std;

class RaceCarDriver{
private:
	Racer* car;

public:
	RaceCarDriver(Racer* p = nullptr): car{p}{}

    struct Point {
        int x;
        int y;

        Point() {
            this->x = -1;
            this->y = -1;
        }
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

        void print() const {
            cout << "(" << x << ", " << y << ")";
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

    bool hasUnvisitedMove(
            const Point& current,
            const set<Point>& visited,
            const Point& endPt) {
        vector<DIRECTION> const DIRECTIONS = {EAST, SOUTH, WEST, NORTH};

        // Check if this cell still has a new place to go.
        for (DIRECTION dir : DIRECTIONS) {
            if (!car->look(dir)) {
                Point next = nextPoint(current, dir);
                if (inBounds(next) && visited.count(next) == 0 && !(next == endPt)) {
                    return true;
                }
            }
        }

        return false;
    }

    void buildBestPath(
            const map<Point, vector<Point>>& graph,
            const Point& start,
            const Point& finish,
            vector<DIRECTION>& bestPath) {
        const int INF = 1000000000;

        // Clear the old path before building a new one.
        bestPath.clear();

        // Collect every cell we know about in "nodes".
        set<Point> nodes;
        for (const auto& entry : graph) {
            nodes.insert(entry.first);
            for (const Point& next : entry.second) {
                nodes.insert(next);
            }
        }

        // ENSURE WE HAVE THE START & FINISH ALREADY
        if (nodes.count(start) == 0 || nodes.count(finish) == 0) {
            return;
        }

        // Maps the distance from the starting point to every point we know about
        map<Point, int> dist;
        map<Point, Point> prev;
        set<Point> unvisited = nodes;

        // Start with every cell at a very large distance.
        for (const Point& node : nodes) {
            dist[node] = INF;
        }
        dist[start] = 0;

        // Pick the closest unvisited cell and "relax" its neighbors.
        while (!unvisited.empty()) {
            Point current = start;
            int bestDist = INF;
            bool found = false;

            for (const Point& node : unvisited) {
                if (dist[node] < bestDist) {
                    bestDist = dist[node];
                    current = node;
                    found = true;
                }
            }

            if (!found || bestDist == INF) {
                break;
            }

            unvisited.erase(current);

            if (current == finish) {
                break;
            }

            // Loop over the points next to that point using the graph we made
            for (const Point& next : graph.at(current)) {
                if (unvisited.count(next) == 0) {
                    continue;
                }

                int alt = dist[current] + 1;
                if (alt < dist[next]) {
                    dist[next] = alt;
                    prev[next] = current;
                }
            }
        }

        if (start == finish || prev.count(finish) == 0) {
            return;
        }

        // Walk backward from the finish to rebuild the path.
        vector<DIRECTION> reversedPath;
        Point step = finish;
        while (!(step == start)) {
            Point parent = prev[step];
            reversedPath.push_back(directionTo(parent, step));
            step = parent;
        }

        reverse(reversedPath.begin(), reversedPath.end());
        bestPath = reversedPath;
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
            map<Point, vector<Point>>& graph,
            vector<DIRECTION>& bestPath,
            size_t& bestPathIdx,
            bool& replayShortestPath,
            const int run) {
        vector<DIRECTION> const DIRECTIONS = {EAST, SOUTH, WEST, NORTH};

        // Decision
        for (DIRECTION dir: DIRECTIONS) {
            if (!car->look(dir)) {
                Point next = nextPoint(current, dir);
                if (inBounds(next) 
                        && visited.count(next) == 0 
                    && (!bestPath.empty() || !(next == endPt))) {
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

            // If we are back at the start and there is nothing left to search,
            // build the shortest path now using dijkstras
            if (parent == startPt && !hasUnvisitedMove(parent, visited, endPt)) {
                buildBestPath(graph, startPt, endPt, bestPath);
                bestPathIdx = 0;
                replayShortestPath = true;

                // Start following the shortest path right away.
                if (bestPathIdx < bestPath.size()) {
                    DIRECTION nextDir = bestPath[bestPathIdx++];
                    current = nextPoint(parent, nextDir);
                    return nextDir;
                }
            }

            return directionTo(child, parent);    
        }
        return EAST;
    }

	DIRECTION nextMoveTeamThree(int run){
        // Points
        static Point startingPoint = Point(0, 0);
        static Point endPoint = Point(-1, -1);
        static Point current = startingPoint;
        static map<Point, vector<Point>> graph;

        // DFS
        static stack<Point> path;
        static set<Point> visited;

        // Best path
        static int lastRun = -1;
        static vector<DIRECTION> bestPath;
        static size_t bestPathIdx = 0;
        static bool replayShortestPath = false;

        // If we go into a new run
        if (run != lastRun) {
            // Save the finish point before resetting the search state.
            if (lastRun == 0 && run == 1) {
                endPoint = current;
                graph.clear();
                bestPath.clear();
                bestPathIdx = 0;
                replayShortestPath = false;
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
                DIRECTION dir = getDFSDir(path, visited, current, endPoint, startingPoint, graph, bestPath, bestPathIdx, replayShortestPath, run);
                return dir;
                break;
            }

            // RUN DFS to create the topology (do not go to the finish)
            case 1: {
                if (replayShortestPath) {
                    if (bestPathIdx < bestPath.size()) {
                        return bestPath[bestPathIdx++];
                    }
                    return NORTH;
                }

                addCurrentCellToGraph(graph, current);
                DIRECTION dir = getDFSDir(path, visited, current, endPoint, startingPoint, graph, bestPath, bestPathIdx, replayShortestPath, run);
                return dir;
                break;
            }

            // Traverse the vector of directions
            case 2: {
                if (bestPathIdx < bestPath.size()) {
                    return bestPath[bestPathIdx++];
                }
                return EAST;
            }

            // Our default is just running DFS, cause why not
            default: {
                DIRECTION dir = getDFSDir(path, visited, current, endPoint, startingPoint, graph, bestPath, bestPathIdx, replayShortestPath, run);
                return dir;
                break;
            }
        }

        // Fallback to satisfy all control paths.
        return EAST;
    }
};


#endif /* RACECARDRIVER_H_ */