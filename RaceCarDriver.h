/*
* Author: Karter Sanamo, Christine Seng, Georgia Rushing,
* Matthew Peterson, Nolan Schirripa (Team Three)
* Assignment Title: Group Project
* Assignment Description: driver for nextMoveTeamThree
* function that solves the maze
* Due Date: 5/06/2026
* Date Created: 4/23/2026
* Date Last Modified: 5/06/2026
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
#include <iostream>
#include <set>
#include <stack>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

struct PointTeamThree {
    int x;
    int y;

    /*
     * description: default constructor for a point
     * return: none, constructor
     * precondition: none
     * postcondition: new PointTeamThree exists with value (-1, -1)
     */
    PointTeamThree() {
        // -1 represents an invalid point, so init to this
        this->x = -1;
        this->y = -1;
    }
    /*
     * description: two parameter constructor for a point
     * return: none, constructor
     * precondition: none
     * postcondition: new PointTeamThree exists with value (x, y)
     */
    PointTeamThree(int x, int y) {
        this->x = x;
        this->y = y;
    }
    /*
     * description: compares two points first by x then y coordinates
     * return: bool, true if left side less than right side
     * precondition: both points' x and y values are not null
     * postcondition: points remain unchanged
     */
    bool operator<(const PointTeamThree& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }
    /*
     * description: compares two points 
     * return: bool, true if x and y values are equal
     * precondition: both points' x and y values are not null
     * postcondition: points remain unchanged
     */
    bool operator==(const PointTeamThree& other) const {
        return x == other.x && y == other.y;
    }

    /*
     * description: prints value of point in (x, y) format
     * return: void
     * precondition: x and y are not null
     * postcondition: points remain unchanged
     */
    void print() const {
        cout << "(" << x << ", " << y << ")";
    }
};

/*
 * description: finds first direction the racer can safely move to without hitting a wall,
    searching in the order: East, South, West, North
 * return: DIRECTION
 * precondition: car is not nullptr
 * postcondition: car does not move and is unchanged
 */
DIRECTION firstOpenDirectionTeamThree(Racer* car);
/*
 * description: adds the current point/node and its edges to the graph
 * return: void
 * precondition: car is not nullptr, graph and currPos are not null and 
    have been initialized
 * postcondition: graph is updated with new information, car does not change
    and does not move
 */
void addCurrentCellToGraphTeamThree(Racer* car, map<PointTeamThree, vector<PointTeamThree>>& graph, PointTeamThree currPos);
/*
 * description: calculates the next point from the point p in the 
    direction d
 * return: PointTeamThree, which is the next point in the direction
 * precondition: p and d have been initialized
 * postcondition: new point returned, p and d are unchanged
 */
PointTeamThree nextPointTeamThree(PointTeamThree p, DIRECTION d);
/*
 * description: removes all entries from both path and visited
 * return: void
 * precondition: path and visited are not null
 * postcondition: path and visited are both empty
 */
void resetDfsStateTeamThree(stack<PointTeamThree>& path, set<PointTeamThree>& visited);
/*
 * description: determines if the current point still has unvisited neighbors
 * return: bool, true if there are unvisited neighbors/moves
 * precondition: car, current, visited, endPointKnown are not null, 
    endPt is not null if endPointKnown == true
 * postcondition: all parameters remain unchanged and car does not move
 */
bool hasUnvisitedMoveTeamThree(Racer* car, const PointTeamThree& current, const set<PointTeamThree>& visited,
    const PointTeamThree& endPt, bool endPointKnown);
/*
 * description: builds the shortest path from start to finish using Dijkstra's algorithm
 * return: void
 * precondition: graph, start, finish, bestPath are not null and have been initialized
 * postcondition: bestPath is updated to contain directions from start to finish, other
    parameters remain unchanged
 */
void buildBestPathTeamThree(const map<PointTeamThree, vector<PointTeamThree>>& graph,
    const PointTeamThree& start, const PointTeamThree& finish, vector<DIRECTION>& bestPath);
/*
 * description: calculates the direction from first to second point
 * return: DIRECTION
 * precondition: from and to are initialized and not null
 * postcondition: from and to remain unchanged
 */
DIRECTION directionToTeamThree(PointTeamThree from, PointTeamThree to);
/*
 * description: runs DFS and gets the next move, either to find the end point or populate
    the rest of the graph information by avoiding the end point, used in the first and second
    runs for exploration
 * return: DIRECTION, next move
 * precondition: all parameters are initialized and not null
 * postcondition: parameters remain unchanged
 */
DIRECTION getDFSDirTeamThree(Racer* car, stack<PointTeamThree>& path, set<PointTeamThree>& visited,
        PointTeamThree& current, PointTeamThree& endPt, PointTeamThree& startPt,
        map<PointTeamThree, vector<PointTeamThree>>& graph, vector<DIRECTION>& bestPath,
        size_t& bestPathIdx, bool& replayShortestPath, bool endPointKnown, const int run);

class RaceCarDriver{
private:
	Racer* car;

public:
	RaceCarDriver(Racer* p = nullptr): car{p}{}

	DIRECTION nextMoveTeamThree(int run){
        // Points
		static PointTeamThree startingPoint = PointTeamThree(0, 0);
        static PointTeamThree endPoint = PointTeamThree(0, 0);
        static PointTeamThree current = startingPoint;
        static map<PointTeamThree, vector<PointTeamThree>> graph;

        // DFS
		static stack<PointTeamThree> path;
        static set<PointTeamThree> visited;

        // Best path
        static int lastRun = -1;
        static vector<DIRECTION> bestPath;
        static size_t bestPathIdx = 0;
        static bool replayShortestPath = false;
        static bool endPointKnown = false;

        if (run != lastRun) {
            if (lastRun == 0 && run == 1) {
                endPoint = current;
                endPointKnown = true;
                graph.clear();
                bestPath.clear();
            }
            if (lastRun == 1 && run == 2) {
                bestPathIdx = 0;
                current = startingPoint;
                replayShortestPath = false;
                lastRun = run;

                if (!bestPath.empty()) {
                    DIRECTION dir = bestPath[bestPathIdx++];
                    current = nextPointTeamThree(current, dir);
                    return dir;
                }

                return firstOpenDirectionTeamThree(car);
            }

            current = startingPoint;
            resetDfsStateTeamThree(path, visited);
            path.push(startingPoint);
            visited.emplace(startingPoint);
            bestPathIdx = 0;
            replayShortestPath = false;
            lastRun = run;
        }

		switch (run) {
            // RUN DFS to only find the finish
            case 0: {
				DIRECTION dir = getDFSDirTeamThree(car, path, visited, current,
				    endPoint, startingPoint, graph, bestPath,
				    bestPathIdx, replayShortestPath, endPointKnown, run);
				return dir;
                break;
            }

            // RUN DFS to create the topology (do not go to the finish)
            case 1: {
                addCurrentCellToGraphTeamThree(car, graph, current);

                // We replay the shortest path after we found topology
                if (replayShortestPath) {
                    if (bestPathIdx < bestPath.size()) {
                        DIRECTION dir = bestPath[bestPathIdx++];
                        current = nextPointTeamThree(current, dir);
                        return dir;
                    }

                    replayShortestPath = false;
                    return firstOpenDirectionTeamThree(car);
                }
                // Once DFS has fully explored the maze and returned to the start
                // Generate the shortest path from start to finish and begin replaying it.
                if (current == startingPoint &&
                    path.size() == 1 &&
                    !hasUnvisitedMoveTeamThree(car, current, visited, endPoint, endPointKnown)) {
                    buildBestPathTeamThree(graph, startingPoint, endPoint, bestPath);
                    bestPathIdx = 0;
                    replayShortestPath = true;

                    if (bestPathIdx < bestPath.size()) {
                        DIRECTION dir = bestPath[bestPathIdx++];
                        current = nextPointTeamThree(current, dir);
                        return dir;
                    }
                }

                DIRECTION dir = getDFSDirTeamThree(car, path, visited, current, endPoint,
                    startingPoint, graph, bestPath, bestPathIdx, replayShortestPath,
                    endPointKnown, run);

                return dir;
            }

            // Traverse the vector of directions
            case 2: {
                if (bestPathIdx < bestPath.size()) {
                    DIRECTION dir = bestPath[bestPathIdx++];
                    current = nextPointTeamThree(current, dir);
                    return dir;
                }

                return NORTH;
            }

            // Our default is just running DFS
            // We should never get here if there are only 3 runs
            default: {
				DIRECTION dir = getDFSDirTeamThree(car, path, visited, current, endPoint,
				    startingPoint, graph, bestPath, bestPathIdx,
				    replayShortestPath, endPointKnown, run);
				return dir;
                break;
            }
        }

        // Fallback to satisfy
        return NORTH;
    }
};

DIRECTION firstOpenDirectionTeamThree(Racer* car) {
    vector<DIRECTION> const DIRECTIONS = {EAST, SOUTH, WEST, NORTH};
    for (DIRECTION dir : DIRECTIONS) {
        if (!car->look(dir)) {
            return dir;
        }
    }
    return NORTH;
}

void addCurrentCellToGraphTeamThree(Racer* car, map<PointTeamThree, vector<PointTeamThree>>& graph, PointTeamThree currPos) {
    graph[currPos] = {};

    if (!car->look(EAST)) {
        graph[currPos].push_back(PointTeamThree(currPos.x + 1, currPos.y));
    }

    if (!car->look(WEST)) {
        graph[currPos].push_back(PointTeamThree(currPos.x - 1, currPos.y));
    }

    if (!car->look(NORTH)) {
        graph[currPos].push_back(PointTeamThree(currPos.x, currPos.y - 1));
    }

    if (!car->look(SOUTH)) {
        graph[currPos].push_back(PointTeamThree(currPos.x, currPos.y + 1));
    }
}

PointTeamThree nextPointTeamThree(PointTeamThree p, DIRECTION d) {
    if (d == EAST) p.x++;
    else if (d == SOUTH) p.y++;
    else if (d == WEST) p.x--;
    else p.y--;
    return p;
}

void resetDfsStateTeamThree(stack<PointTeamThree>& path, set<PointTeamThree>& visited) {
    while (!path.empty()) path.pop();
    visited.clear();
}

bool hasUnvisitedMoveTeamThree(
    Racer* car,
        const PointTeamThree& current,
        const set<PointTeamThree>& visited,
        const PointTeamThree& endPt,
        bool endPointKnown) {
    vector<DIRECTION> const DIRECTIONS = {EAST, SOUTH, WEST, NORTH};

    // Check if this cell still has a new place to go
    for (DIRECTION dir : DIRECTIONS) {
        if (!car->look(dir)) {
            PointTeamThree next = nextPointTeamThree(current, dir);
            // If the neighbor is unvisited and it is not the end point (if known), state there
            // are still unvisited moves
            if (visited.count(next) == 0 && (!endPointKnown || !(next == endPt))) {
                return true;
            }
        }
    }

    return false;
}

void buildBestPathTeamThree(
        const map<PointTeamThree, vector<PointTeamThree>>& graph,
        const PointTeamThree& start,
        const PointTeamThree& finish,
        vector<DIRECTION>& bestPath) {
    // this value can be any number greater or equal to 2 since each edge
    // has a weight of 1, if edges with different weights were to be implemented
    // this can be changed to INT_MAX in climits
    const int INF = 1000000; 

    // Clear the old path before building a new one
    bestPath.clear();

    // Collect every cell we know about in "nodes"
    set<PointTeamThree> nodes;
    for (const auto& entry : graph) {
        nodes.insert(entry.first);
        for (const PointTeamThree& next : entry.second) {
            nodes.insert(next);
        }
    }

    // ENSURE WE HAVE THE START & FINISH ALREADY
    if (nodes.count(start) == 0 || nodes.count(finish) == 0) {
        return;
    }

    // Init data structures
    // Maps the distance from the starting point to every point we know about
    map<PointTeamThree, int> dist;
    map<PointTeamThree, PointTeamThree> prev;
    set<PointTeamThree> unvisited = nodes;

    // Start with every cell at a very large distance. (Dijkstra's standard)
    // INF == unknown
    for (const PointTeamThree& node : nodes) {
        dist[node] = INF;
    }
    dist[start] = 0;
    // dist from start->start = 0

    // Pick the closest unvisited cell and "relax" its neighbors.
    while (!unvisited.empty()) {
        PointTeamThree current = start;
        int bestDist = INF;
        bool found = false;

        // relaxation: if we can get to a node with a shorter path,
        // update the path
        for (const PointTeamThree& node : unvisited) {
            if (dist[node] < bestDist) {
                bestDist = dist[node];
                current = node;
                found = true;
            }
        }

        // break if node is unreachable
        if (!found || bestDist == INF) {
            break;
        }

        unvisited.erase(current);

        if (current == finish) {
            break;
        }

        // Loop over the points next to that point using the graph we made
        for (const PointTeamThree& next : graph.at(current)) {
            // skip unreachable nodes
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
    PointTeamThree step = finish;
    while (!(step == start)) {
        PointTeamThree parent = prev[step];
        reversedPath.push_back(directionToTeamThree(parent, step));
        step = parent;
    }

    reverse(reversedPath.begin(), reversedPath.end());
    bestPath = reversedPath;
}

DIRECTION directionToTeamThree(PointTeamThree from, PointTeamThree to) {
    if (to.x > from.x) return EAST;
    if (to.x < from.x) return WEST;
    if (to.y > from.y) return SOUTH;
    return NORTH;
}

DIRECTION getDFSDirTeamThree(
        Racer* car,
        stack<PointTeamThree>& path,
        set<PointTeamThree>& visited,
        PointTeamThree& current,
        PointTeamThree& endPt,
        PointTeamThree& startPt,
        map<PointTeamThree, vector<PointTeamThree>>& graph,
        vector<DIRECTION>& bestPath,
        size_t& bestPathIdx,
        bool& replayShortestPath,
        bool endPointKnown,
        const int run) {
    vector<DIRECTION> const DIRECTIONS = {EAST, SOUTH, WEST, NORTH};

    // Decision
    for (DIRECTION dir: DIRECTIONS) {
        if (!car->look(dir)) {
            PointTeamThree next = nextPointTeamThree(current, dir);
            if (visited.count(next) == 0
                && (!endPointKnown || !bestPath.empty() || !(next == endPt))) {
                path.push(next);
                visited.emplace(next);
                current = next;
                return dir;
            }
        }
    }

    // Backtracking
    if (path.size() > 1) {
        PointTeamThree child = current;
        path.pop();
        PointTeamThree parent = path.top();
        current = parent;

        DIRECTION dir = directionToTeamThree(child, parent);
        return dir;    
    }

    return firstOpenDirectionTeamThree(car);
}

#endif /* RACECARDRIVER_H_ */