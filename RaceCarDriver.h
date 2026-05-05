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

    bool inBounds(const pair<int,int>& p) {
        return p.first >= 0 && p.second >= 0 && p.first < col && p.second < row;
    }

    DIRECTION directionTo(pair<int, int> from, pair<int,int> to) {
        if (to.first > from.first) return EAST;
        if (to.first < from.first) return WEST;
        if (to.second > from.second) return SOUTH;
        return NORTH;
    }

    void addEdges(map<pair<int,int>, vector<pair<pair<int,int>, DIRECTION>>> &graph, pair<int,int> current, Racer* car){
        vector<DIRECTION> dirs = {EAST, SOUTH, WEST, NORTH};

        for(DIRECTION d : dirs){
            if(!car->look(d)){
                pair<int,int> next = current;

                if(d == EAST) next.first++;
                if(d == WEST) next.first--;
                if(d == SOUTH) next.second++;
                if(d == NORTH) next.second--;

                graph[current].push_back({next, d});
            }
        }
    }

    pair<int, int> nextPoint(pair<int, int> p, DIRECTION d) {
        if (d == EAST) p.first++;
        else if (d == SOUTH) p.second++;
        else if (d == WEST) p.first--;
        else p.second--;
        return p;
    }

public:
    RaceCarDriver(Racer* p = nullptr): car{p} {}

    DIRECTION nextMoveTeamThree(int run = 0) {
        static map<pair<int, int>, vector<pair<pair<int,int>, DIRECTION>>> graph;
        static set<pair<int,int>> visited;
        static stack<pair<int,int>> path;
        static vector<DIRECTION> bestPath;

        static bool solutionReady = false;
        static size_t replayIndex = 0;
        static pair<int,int> goal = {-1, -1};
        static int x = 0;
        static int y = 0;
        vector<DIRECTION> directions = {EAST, SOUTH, WEST, NORTH};

        if (car == nullptr) return EAST;

        pair <int, int> current = {x, y};

        // dfs explore and build graph
        if (run == 0) {

            if (path.empty()) {
                path.push(current);
                visited.insert(current);
            }

            for (DIRECTION dir: directions) {
                if (!car->look(dir)) {
                    pair<int, int> next = nextPoint(current, dir);
                    
                    if (inBounds(next) && !visited.count(next)) {
                        path.push(next);
                        visited.insert(next);
                        
                        x = next.first;
                        y = next.second;

                        return dir;
                    }
                }
            }

            if (path.size() > 1) {
                path.pop();
                pair<int, int> parent = path.top();
                return directionTo(current, parent);
            }
        }

        // run bfs
        if (run == 1) {

        }

        // shortest path
        if (run == 2 && replayIndex < bestPath.size()) {
            DIRECTION d = bestPath[replayIndex];
            replayIndex++;
            return d;
        }

        return EAST;
    }
};


#endif /* RACECARDRIVER_H_ */