/*
* RaceCarDriverBFS.h
 *
 *  Created on: Spring, 2026
 *      Author: bill_booth
 */

 #ifndef RACECARDRIVER_H_
 #define RACECARDRIVER_H_
 
 #include "Racer.h"
 #include <climits>
 #include <queue>
 #include <set>
 #include <stack>
 #include <vector>
 
 using namespace std;
 
 enum State {
         BFS,
         REPLAY
     };
 
 class RaceCarDriver {
 private:
 
     Racer* car;
     static State mode;
 
     // BFS state
     static stack<point> path;
     static set<pair<int,int>> visited;
 
     // BFS storage
     static bool wallKnown[row][col][4];
     static bool wallBlocked[row][col][4];
 
     static int distScratch[row][col];
 
     // Solution storage for run 2 & 3
     static vector<DIRECTION> solvedDirs;
 
     // Replay state for run 2 & 3
     static size_t replayIndex;
 
     // Tracking
     static point lastPos;
     static DIRECTION lastDir;
 
     vector<DIRECTION> const DIRECTIONS = {EAST, SOUTH, WEST, NORTH};
 
     int dirIndex(DIRECTION d) const {
         return static_cast<int>(d);
     }
 
     DIRECTION opposite(DIRECTION d) const {
         if (d == NORTH) return SOUTH;
         if (d == SOUTH) return NORTH;
         if (d == EAST) return WEST;
         return EAST;
     }
 
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
 
     void resetBfsState() {
         while (!path.empty()) path.pop();
         visited.clear();
         solvedDirs.clear();
         replayIndex = 0;
         for (int y = 0; y < row; y++) {
             for (int x = 0; x < col; x++) {
                 for (int k = 0; k < 4; k++) {
                     wallKnown[y][x][k] = false;
                     wallBlocked[y][x][k] = false;
                }
            }
        }
     }
 
     void clearDist() {
         for (int y = 0; y < row; y++)
             for (int x = 0; x < col; x++)
                 distScratch[y][x] = INT_MAX;
     }
 
     void revealWallsHere(point current) {
         for (DIRECTION d : DIRECTIONS) {
             int i = dirIndex(d);
             if (!wallKnown[current.y][current.x][i]) {
                 wallKnown[current.y][current.x][i] = true;
                 wallBlocked[current.y][current.x][i] = car->look(d);
             }
         }
     }
 
     // BFS from start only — no assumed goal coordinates.
     void bfsDistFromStart() {
         clearDist();
         const point start(0, 0);
         if (!inBounds(start))
             return;
 
         queue<point> q;
         distScratch[0][0] = 0;
         q.push(start);
 
         while (!q.empty()) {
             point u = q.front();
             q.pop();
             int du = distScratch[u.y][u.x];
             for (DIRECTION d : DIRECTIONS) {
                 point v = nextPoint(u, d);
                 if (!inBounds(v))
                     continue;
                 int ui = dirIndex(d);
                 bool known = wallKnown[u.y][u.x][ui];
                 bool blocked = wallBlocked[u.y][u.x][ui];
                 if (known && blocked)
                     continue;
                 if (distScratch[v.y][v.x] == INT_MAX) {
                     distScratch[v.y][v.x] = du + 1;
                     q.push(v);
                 }
             }
         }
     }
 
     // Try one forward step using BFS distances; same side effects as original pickDir on success.
     bool pickDirBfs(point current, DIRECTION& outDir) {
         revealWallsHere(current);
 
         bfsDistFromStart();
 
         // Prefer unvisited neighbor farthest from start in BFS metric (frontier expansion).
         int best = -1;
         DIRECTION bestD = EAST;
         for (DIRECTION d : DIRECTIONS) {
             point nxt = nextPoint(current, d);
             if (!inBounds(nxt))
                 continue;
             int i = dirIndex(d);
             if (wallKnown[current.y][current.x][i] && wallBlocked[current.y][current.x][i])
                 continue;
             if (visited.count({nxt.x, nxt.y}) != 0)
                 continue;
             int nd = distScratch[nxt.y][nxt.x];
             if (nd != INT_MAX && nd > best) {
                 best = nd;
                 bestD = d;
             }
         }
         if (best < 0)
             return false;
 
         point next = nextPoint(current, bestD);
         path.push(next);
         visited.emplace(next.x, next.y);
         lastPos = current;
         lastDir = bestD;
         outDir = bestD;
         solvedDirs.push_back(bestD);
         return true;
     }
 
 public:
     RaceCarDriver(Racer* p = nullptr): car{p} {}
 
     DIRECTION nextMoveTeamThree() {
         point current = car->getLocation();
 
         if (mode == BFS && atStart(current) && !atStart(lastPos)) {
             if (path.size() > 1) {
                 if (atStart(path.top())) {
                     path.pop();
                     if (!solvedDirs.empty()) {
                         solvedDirs.pop_back();
                     }
                 }
                 if (!solvedDirs.empty()) {
                     mode = REPLAY;
                     replayIndex = 0;
                     lastPos = current;
                 }
                 else {
                     resetBfsState();
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
 
         // BFS-driven exploration (same walk/backtrack structure as original DFS driver)
         if (path.empty()) {
             path.push(current);
             visited.emplace(current.x, current.y);
         }
 
         DIRECTION dir;
         if (pickDirBfs(current, dir)) {
             lastPos = current;
             return dir;
         }
 
         if (path.size() > 1) {
             path.pop();
             if (!solvedDirs.empty()) {
                 solvedDirs.pop_back();
             }
             point parent = path.top();
             lastPos = current;
             return directionTo(current, parent);
         }
 
         lastPos = current;
         car->die();
         return EAST;
     }
 };
 
 stack<point> RaceCarDriver::path;
 set<pair<int,int>> RaceCarDriver::visited;
 bool RaceCarDriver::wallKnown[row][col][4];
 bool RaceCarDriver::wallBlocked[row][col][4];
 int RaceCarDriver::distScratch[row][col];
 vector<DIRECTION> RaceCarDriver::solvedDirs;
 State RaceCarDriver::mode = BFS;
 size_t RaceCarDriver::replayIndex = 0;
 point RaceCarDriver::lastPos = point(-1, -1);
 DIRECTION RaceCarDriver::lastDir = EAST;
 
 #endif /* RACECARDRIVER_H_ */
 