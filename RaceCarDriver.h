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
#include <iostream>
#include <set>
#include <stack>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

struct TeamThreePoint {
    int x;
    int y;

    TeamThreePoint() {
        // -1 represents an invalid point, so init to this
        this->x = -1;
        this->y = -1;
    }
    TeamThreePoint(int x, int y) {
        this->x = x;
        this->y = y;
    }
    bool operator<(const TeamThreePoint& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }
    bool operator==(const TeamThreePoint& other) const {
        return x == other.x && y == other.y;
    }

    void print() const {
        cout << "(" << x << ", " << y << ")";
    }
};


void addCurrentCellToGraph(Racer* car, map<TeamThreePoint, vector<TeamThreePoint>>& graph, TeamThreePoint currPos);
TeamThreePoint nextPoint(TeamThreePoint p, DIRECTION d);
void resetDfsState(stack<TeamThreePoint>& path, set<TeamThreePoint>& visited);
bool hasUnvisitedMove(Racer* car, const TeamThreePoint& current, const set<TeamThreePoint>& visited, const TeamThreePoint& endPt, bool endPointKnown);
void buildBestPath(const map<TeamThreePoint, vector<TeamThreePoint>>& graph, const TeamThreePoint& start, const TeamThreePoint& finish, vector<DIRECTION>& bestPath);
DIRECTION directionTo(TeamThreePoint from, TeamThreePoint to);
DIRECTION getDFSDir(Racer* car,
        stack<TeamThreePoint>& path,
        set<TeamThreePoint>& visited,
        TeamThreePoint& current,
        TeamThreePoint& endPt,
        TeamThreePoint& startPt,
        map<TeamThreePoint, vector<TeamThreePoint>>& graph,
        vector<DIRECTION>& bestPath,
        size_t& bestPathIdx,
        bool& replayShortestPath,
        bool endPointKnown,
        const int run);

class RaceCarDriver{
private:
	Racer* car;

public:
	RaceCarDriver(Racer* p = nullptr): car{p}{}

	DIRECTION nextMoveTeamThree(int run){
        // Points
		static TeamThreePoint startingPoint = TeamThreePoint(0, 0);
        static TeamThreePoint endPoint = TeamThreePoint(0, 0);
        static TeamThreePoint current = startingPoint;
        static map<TeamThreePoint, vector<TeamThreePoint>> graph;

        // DFS
		static stack<TeamThreePoint> path;
        static set<TeamThreePoint> visited;

        // Best path
        static int lastRun = -1;
        static vector<DIRECTION> bestPath;
        static size_t bestPathIdx = 0;
        static bool replayShortestPath = false;
        static bool endPointKnown = false;

        if (run != lastRun) {
            // Save the finish point before resetting the search state.
            if (lastRun == 0 && run == 1) {
                endPoint = current;
                endPointKnown = true;
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
				DIRECTION dir = getDFSDir(car, path, visited, current, endPoint, startingPoint, graph, bestPath, bestPathIdx, replayShortestPath, endPointKnown, run);
				return dir;
                break;
            }

            // RUN DFS to create the topology (do not go to the finish)
            case 1: {
                if (replayShortestPath) {
                    // Running dijkstra's after getting back to the start
                    if (bestPathIdx < bestPath.size()) {
                        DIRECTION dir = bestPath[bestPathIdx++];
                        current = nextPoint(current, dir);
                        return dir;
                    }
                    return NORTH;
                }

                addCurrentCellToGraph(car, graph, current);
                DIRECTION dir = getDFSDir(car, path, visited, current, endPoint, startingPoint, graph, bestPath, bestPathIdx, replayShortestPath, endPointKnown, run);
				return dir;
                break;
            }

            // Traverse the vector of directions
            case 2: {
                if (bestPathIdx < bestPath.size()) {
                    DIRECTION dir = bestPath[bestPathIdx++];
                    current = nextPoint(current, dir);
                    return dir;
                }
                return EAST;
            }

            // Our default is just running DFS, cause why not
            // We should never get here if he only runs 3 runs
            default: {
				DIRECTION dir = getDFSDir(car, path, visited, current, endPoint, startingPoint, graph, bestPath, bestPathIdx, replayShortestPath, endPointKnown, run);
				return dir;
                break;
            }
        }

        // Fallback to satisfy all control paths.
        return NORTH;
    }
};

void addCurrentCellToGraph(Racer* car, map<TeamThreePoint, vector<TeamThreePoint>>& graph, TeamThreePoint currPos) {
    graph[currPos] = {};

    if (!car->look(EAST)) {
        graph[currPos].push_back(TeamThreePoint(currPos.x + 1, currPos.y));
    }

    if (!car->look(WEST)) {
        graph[currPos].push_back(TeamThreePoint(currPos.x - 1, currPos.y));
    }

    if (!car->look(NORTH)) {
        graph[currPos].push_back(TeamThreePoint(currPos.x, currPos.y - 1));
    }

    if (!car->look(SOUTH)) {
        graph[currPos].push_back(TeamThreePoint(currPos.x, currPos.y + 1));
    }
}

TeamThreePoint nextPoint(TeamThreePoint p, DIRECTION d) {
    if (d == EAST) p.x++;
    else if (d == SOUTH) p.y++;
    else if (d == WEST) p.x--;
    else p.y--;
    return p;
}

void resetDfsState(stack<TeamThreePoint>& path, set<TeamThreePoint>& visited) {
    while (!path.empty()) path.pop();
    visited.clear();
}

bool hasUnvisitedMove(
    Racer* car,
        const TeamThreePoint& current,
        const set<TeamThreePoint>& visited,
        const TeamThreePoint& endPt,
        bool endPointKnown) {
    vector<DIRECTION> const DIRECTIONS = {EAST, SOUTH, WEST, NORTH};

    // Check if this cell still has a new place to go.
    for (DIRECTION dir : DIRECTIONS) {
        if (!car->look(dir)) {
            TeamThreePoint next = nextPoint(current, dir);
            if (visited.count(next) == 0 && (!endPointKnown || !(next == endPt))) {
                return true;
            }
        }
    }

    return false;
}

void buildBestPath(
        const map<TeamThreePoint, vector<TeamThreePoint>>& graph,
        const TeamThreePoint& start,
        const TeamThreePoint& finish,
        vector<DIRECTION>& bestPath) {
    const int INF = 1000000000;

    // Clear the old path before building a new one.
    bestPath.clear();

    // Collect every cell we know about in "nodes".
    set<TeamThreePoint> nodes;
    for (const auto& entry : graph) {
        nodes.insert(entry.first);
        for (const TeamThreePoint& next : entry.second) {
            nodes.insert(next);
        }
    }

    // ENSURE WE HAVE THE START & FINISH ALREADY
    if (nodes.count(start) == 0 || nodes.count(finish) == 0) {
        return;
    }

    // Init data structures
    // Maps the distance from the starting point to every point we know about
    map<TeamThreePoint, int> dist;
    map<TeamThreePoint, TeamThreePoint> prev;
    set<TeamThreePoint> unvisited = nodes;

    // Start with every cell at a very large distance. (dijkstra's standard)
    // INF == unknown
    for (const TeamThreePoint& node : nodes) {
        dist[node] = INF;
    }
    dist[start] = 0;
    // dist from start->start = 0

    // Pick the closest unvisited cell and "relax" its neighbors.
    while (!unvisited.empty()) {
        TeamThreePoint current = start;
        int bestDist = INF;
        bool found = false;

        for (const TeamThreePoint& node : unvisited) {
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
        for (const TeamThreePoint& next : graph.at(current)) {
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
    TeamThreePoint step = finish;
    while (!(step == start)) {
        TeamThreePoint parent = prev[step];
        reversedPath.push_back(directionTo(parent, step));
        step = parent;
    }

    reverse(reversedPath.begin(), reversedPath.end());
    bestPath = reversedPath;
}

DIRECTION directionTo(TeamThreePoint from, TeamThreePoint to) {
    if (to.x > from.x) return EAST;
    if (to.x < from.x) return WEST;
    if (to.y > from.y) return SOUTH;
    return NORTH;
}

DIRECTION getDFSDir(
        Racer* car,
        stack<TeamThreePoint>& path,
        set<TeamThreePoint>& visited,
        TeamThreePoint& current,
        TeamThreePoint& endPt,
        TeamThreePoint& startPt,
        map<TeamThreePoint, vector<TeamThreePoint>>& graph,
        vector<DIRECTION>& bestPath,
        size_t& bestPathIdx,
        bool& replayShortestPath,
        bool endPointKnown,
        const int run) {
    vector<DIRECTION> const DIRECTIONS = {EAST, SOUTH, WEST, NORTH};

    // Decision
    for (DIRECTION dir: DIRECTIONS) {
        if (!car->look(dir)) {
            TeamThreePoint next = nextPoint(current, dir);
            if (visited.count(next) == 0 
                && (!endPointKnown || !bestPath.empty() || !(next == endPt))
                && !(next == startPt)) {
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

        // If we are back at the start and there is nothing left to search,
        // build the shortest path now using dijkstras
        if (parent == startPt && !hasUnvisitedMove(car, parent, visited, endPt, endPointKnown)) {
            buildBestPath(graph, startPt, endPt, bestPath);
            bestPathIdx = 1;
            replayShortestPath = true;
            //if (bestPathIdx < bestPath.size()) {
            //    DIRECTION nextDir = bestPath[bestPathIdx++];
            //    current = nextPoint(parent, nextDir);
            //    return nextDir;
            //}
        }

        DIRECTION dir = directionTo(child, parent);
        return dir;    
    }

    return EAST;
}

#endif /* RACECARDRIVER_H_ */