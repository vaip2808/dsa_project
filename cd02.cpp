#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <utility>
#include <numeric>
#include <tuple>
#include <limits>

using namespace std;

// ===================== SEGMENT TREE =====================
class SegmentTree
{
private:
    vector<pair<int, int>> a;
    vector<long long> sumAttackTree, sumHealthTree;
    vector<int> maxAttackTreeIndex, maxHealthTreeIndex;
    vector<int> minAttackTreeIndex, minHealthTreeIndex;
    vector<int> gcdAttackTree, gcdHealthTree;
    vector<long long> lcmAttackTree, lcmHealthTree;

    void buildSumAttack(int node, int start, int end)
    {
        if (start == end) { sumAttackTree[node] = a[start].first; return; }
        int mid = (start + end) / 2;
        buildSumAttack(2*node+1, start, mid);
        buildSumAttack(2*node+2, mid+1, end);
        sumAttackTree[node] = sumAttackTree[2*node+1] + sumAttackTree[2*node+2];
    }
    void buildSumHealth(int node, int start, int end)
    {
        if (start == end) { sumHealthTree[node] = a[start].second; return; }
        int mid = (start + end) / 2;
        buildSumHealth(2*node+1, start, mid);
        buildSumHealth(2*node+2, mid+1, end);
        sumHealthTree[node] = sumHealthTree[2*node+1] + sumHealthTree[2*node+2];
    }
    long long lcm(long long a, long long b)
    {
        if (a == 0 || b == 0) return 0;
        return a * (b / __gcd(a, b));
    }
    long long querySumAttack(int node, int start, int end, int l, int r)
    {
        if (start > r || end < l) return 0;
        if (l <= start && r >= end) return sumAttackTree[node];
        int mid = (start + end) / 2;
        return querySumAttack(2*node+1, start, mid, l, r) +
               querySumAttack(2*node+2, mid+1, end, l, r);
    }
    long long querySumHealth(int node, int start, int end, int l, int r)
    {
        if (start > r || end < l) return 0;
        if (l <= start && r >= end) return sumHealthTree[node];
        int mid = (start + end) / 2;
        return querySumHealth(2*node+1, start, mid, l, r) +
               querySumHealth(2*node+2, mid+1, end, l, r);
    }
    void buildMaxAttack(int node, int start, int end)
    {
        if (start == end) { maxAttackTreeIndex[node] = start; return; }
        int mid = (start + end) / 2;
        buildMaxAttack(2*node+1, start, mid);
        buildMaxAttack(2*node+2, mid+1, end);
        maxAttackTreeIndex[node] = (a[maxAttackTreeIndex[2*node+1]].first > a[maxAttackTreeIndex[2*node+2]].first)
                                    ? maxAttackTreeIndex[2*node+1] : maxAttackTreeIndex[2*node+2];
    }
    void buildMaxHealth(int node, int start, int end)
    {
        if (start == end) { maxHealthTreeIndex[node] = start; return; }
        int mid = (start + end) / 2;
        buildMaxHealth(2*node+1, start, mid);
        buildMaxHealth(2*node+2, mid+1, end);
        maxHealthTreeIndex[node] = (a[maxHealthTreeIndex[2*node+1]].second > a[maxHealthTreeIndex[2*node+2]].second)
                                    ? maxHealthTreeIndex[2*node+1] : maxHealthTreeIndex[2*node+2];
    }
    void buildMinAttack(int node, int start, int end)
    {
        if (start == end) { minAttackTreeIndex[node] = start; return; }
        int mid = (start + end) / 2;
        buildMinAttack(2*node+1, start, mid);
        buildMinAttack(2*node+2, mid+1, end);
        minAttackTreeIndex[node] = (a[minAttackTreeIndex[2*node+1]].first < a[minAttackTreeIndex[2*node+2]].first)
                                    ? minAttackTreeIndex[2*node+1] : minAttackTreeIndex[2*node+2];
    }
    void buildMinHealth(int node, int start, int end)
    {
        if (start == end) { minHealthTreeIndex[node] = start; return; }
        int mid = (start + end) / 2;
        buildMinHealth(2*node+1, start, mid);
        buildMinHealth(2*node+2, mid+1, end);
        minHealthTreeIndex[node] = (a[minHealthTreeIndex[2*node+1]].second < a[minHealthTreeIndex[2*node+2]].second)
                                    ? minHealthTreeIndex[2*node+1] : minHealthTreeIndex[2*node+2];
    }
    int queryMaxAttackIndex(int node, int start, int end, int l, int r)
    {
        if (start > r || end < l) return -1;
        if (l <= start && r >= end) return maxAttackTreeIndex[node];
        int mid = (start + end) / 2;
        int left  = queryMaxAttackIndex(2*node+1, start, mid, l, r);
        int right = queryMaxAttackIndex(2*node+2, mid+1, end, l, r);
        if (left  == -1) return right;
        if (right == -1) return left;
        return (a[left].first > a[right].first) ? left : right;
    }
    int queryMaxHealthIndex(int node, int start, int end, int l, int r)
    {
        if (start > r || end < l) return -1;
        if (l <= start && r >= end) return maxHealthTreeIndex[node];
        int mid = (start + end) / 2;
        int left  = queryMaxHealthIndex(2*node+1, start, mid, l, r);
        int right = queryMaxHealthIndex(2*node+2, mid+1, end, l, r);
        if (left  == -1) return right;
        if (right == -1) return left;
        return (a[left].second > a[right].second) ? left : right;
    }
    int queryMinAttackIndex(int node, int start, int end, int l, int r)
    {
        if (start > r || end < l) return -1;
        if (l <= start && r >= end) return minAttackTreeIndex[node];
        int mid = (start + end) / 2;
        int left  = queryMinAttackIndex(2*node+1, start, mid, l, r);
        int right = queryMinAttackIndex(2*node+2, mid+1, end, l, r);
        if (left  == -1) return right;
        if (right == -1) return left;
        return (a[left].first < a[right].first) ? left : right;
    }
    int queryMinHealthIndex(int node, int start, int end, int l, int r)
    {
        if (start > r || end < l) return -1;
        if (l <= start && r >= end) return minHealthTreeIndex[node];
        int mid = (start + end) / 2;
        int left  = queryMinHealthIndex(2*node+1, start, mid, l, r);
        int right = queryMinHealthIndex(2*node+2, mid+1, end, l, r);
        if (left  == -1) return right;
        if (right == -1) return left;
        return (a[left].second < a[right].second) ? left : right;
    }
    void buildGcdAttack(int node, int start, int end)
    {
        if (start == end) { gcdAttackTree[node] = a[start].first; return; }
        int mid = (start + end) / 2;
        buildGcdAttack(2*node+1, start, mid);
        buildGcdAttack(2*node+2, mid+1, end);
        gcdAttackTree[node] = __gcd(gcdAttackTree[2*node+1], gcdAttackTree[2*node+2]);
    }
    void buildGcdHealth(int node, int start, int end)
    {
        if (start == end) { gcdHealthTree[node] = a[start].second; return; }
        int mid = (start + end) / 2;
        buildGcdHealth(2*node+1, start, mid);
        buildGcdHealth(2*node+2, mid+1, end);
        gcdHealthTree[node] = __gcd(gcdHealthTree[2*node+1], gcdHealthTree[2*node+2]);
    }
    void buildLcmAttack(int node, int start, int end)
    {
        if (start == end) { lcmAttackTree[node] = a[start].first; return; }
        int mid = (start + end) / 2;
        buildLcmAttack(2*node+1, start, mid);
        buildLcmAttack(2*node+2, mid+1, end);
        lcmAttackTree[node] = lcm(lcmAttackTree[2*node+1], lcmAttackTree[2*node+2]);
    }
    void buildLcmHealth(int node, int start, int end)
    {
        if (start == end) { lcmHealthTree[node] = a[start].second; return; }
        int mid = (start + end) / 2;
        buildLcmHealth(2*node+1, start, mid);
        buildLcmHealth(2*node+2, mid+1, end);
        lcmHealthTree[node] = lcm(lcmHealthTree[2*node+1], lcmHealthTree[2*node+2]);
    }
    int queryGcdAttack(int node, int start, int end, int l, int r)
    {
        if (start > r || end < l) return 0;
        if (l <= start && r >= end) return gcdAttackTree[node];
        int mid = (start + end) / 2;
        return __gcd(queryGcdAttack(2*node+1, start, mid, l, r),
                     queryGcdAttack(2*node+2, mid+1, end, l, r));
    }
    int queryGcdHealth(int node, int start, int end, int l, int r)
    {
        if (start > r || end < l) return 0;
        if (l <= start && r >= end) return gcdHealthTree[node];
        int mid = (start + end) / 2;
        return __gcd(queryGcdHealth(2*node+1, start, mid, l, r),
                     queryGcdHealth(2*node+2, mid+1, end, l, r));
    }
    long long queryLcmAttack(int node, int start, int end, int l, int r)
    {
        if (start > r || end < l) return 1;
        if (l <= start && r >= end) return lcmAttackTree[node];
        int mid = (start + end) / 2;
        return lcm(queryLcmAttack(2*node+1, start, mid, l, r),
                   queryLcmAttack(2*node+2, mid+1, end, l, r));
    }
    long long queryLcmHealth(int node, int start, int end, int l, int r)
    {
        if (start > r || end < l) return 1;
        if (l <= start && r >= end) return lcmHealthTree[node];
        int mid = (start + end) / 2;
        return lcm(queryLcmHealth(2*node+1, start, mid, l, r),
                   queryLcmHealth(2*node+2, mid+1, end, l, r));
    }
    void updateSumAttack(int idx, int start, int end, int pos, int value)
    {
        if (start == end) { sumAttackTree[idx] = value; return; }
        int mid = (start + end) / 2;
        if (pos <= mid) updateSumAttack(2*idx+1, start, mid, pos, value);
        else            updateSumAttack(2*idx+2, mid+1, end, pos, value);
        sumAttackTree[idx] = sumAttackTree[2*idx+1] + sumAttackTree[2*idx+2];
    }
    void updateSumHealth(int idx, int start, int end, int pos, int value)
    {
        if (start == end) { sumHealthTree[idx] = value; return; }
        int mid = (start + end) / 2;
        if (pos <= mid) updateSumHealth(2*idx+1, start, mid, pos, value);
        else            updateSumHealth(2*idx+2, mid+1, end, pos, value);
        sumHealthTree[idx] = sumHealthTree[2*idx+1] + sumHealthTree[2*idx+2];
    }
    void updateMaxAttackIndex(int index, int value, int start, int end, int node)
    {
        if (start == end) { maxAttackTreeIndex[node] = start; return; }
        int mid = (start + end) / 2;
        if (index <= mid) updateMaxAttackIndex(index, value, start, mid, 2*node+1);
        else              updateMaxAttackIndex(index, value, mid+1, end, 2*node+2);
        maxAttackTreeIndex[node] = (a[maxAttackTreeIndex[2*node+1]].first > a[maxAttackTreeIndex[2*node+2]].first)
                                    ? maxAttackTreeIndex[2*node+1] : maxAttackTreeIndex[2*node+2];
    }
    void updateMaxHealthIndex(int index, int value, int start, int end, int node)
    {
        if (start == end) { maxHealthTreeIndex[node] = index; return; }
        int mid = (start + end) / 2;
        if (index <= mid) updateMaxHealthIndex(index, value, start, mid, 2*node+1);
        else              updateMaxHealthIndex(index, value, mid+1, end, 2*node+2);
        maxHealthTreeIndex[node] = (a[maxHealthTreeIndex[2*node+1]].second > a[maxHealthTreeIndex[2*node+2]].second)
                                    ? maxHealthTreeIndex[2*node+1] : maxHealthTreeIndex[2*node+2];
    }
    void updateMinAttackIndex(int index, int value, int start, int end, int node)
    {
        if (start == end) { minAttackTreeIndex[node] = index; return; }
        int mid = (start + end) / 2;
        if (index <= mid) updateMinAttackIndex(index, value, start, mid, 2*node+1);
        else              updateMinAttackIndex(index, value, mid+1, end, 2*node+2);
        minAttackTreeIndex[node] = (a[minAttackTreeIndex[2*node+1]].first < a[minAttackTreeIndex[2*node+2]].first)
                                    ? minAttackTreeIndex[2*node+1] : minAttackTreeIndex[2*node+2];
    }
    void updateMinHealthIndex(int index, int value, int start, int end, int node)
    {
        if (start == end) { minHealthTreeIndex[node] = index; return; }
        int mid = (start + end) / 2;
        if (index <= mid) updateMinHealthIndex(index, value, start, mid, 2*node+1);
        else              updateMinHealthIndex(index, value, mid+1, end, 2*node+2);
        minHealthTreeIndex[node] = (a[minHealthTreeIndex[2*node+1]].second < a[minHealthTreeIndex[2*node+2]].second)
                                    ? minHealthTreeIndex[2*node+1] : minHealthTreeIndex[2*node+2];
    }
    void updateGCDAttack(int idx, int start, int end, int pos, int value)
    {
        if (start == end) { gcdAttackTree[idx] = value; return; }
        int mid = (start + end) / 2;
        if (pos <= mid) updateGCDAttack(2*idx+1, start, mid, pos, value);
        else            updateGCDAttack(2*idx+2, mid+1, end, pos, value);
        gcdAttackTree[idx] = __gcd(gcdAttackTree[2*idx+1], gcdAttackTree[2*idx+2]);
    }
    void updateGCDHealth(int idx, int start, int end, int pos, int value)
    {
        if (start == end) { gcdHealthTree[idx] = value; return; }
        int mid = (start + end) / 2;
        if (pos <= mid) updateGCDHealth(2*idx+1, start, mid, pos, value);
        else            updateGCDHealth(2*idx+2, mid+1, end, pos, value);
        gcdHealthTree[idx] = __gcd(gcdHealthTree[2*idx+1], gcdHealthTree[2*idx+2]);
    }
    void updateLCMAttack(int idx, int start, int end, int pos, int value)
    {
        if (start == end) { lcmAttackTree[idx] = value; return; }
        int mid = (start + end) / 2;
        if (pos <= mid) updateLCMAttack(2*idx+1, start, mid, pos, value);
        else            updateLCMAttack(2*idx+2, mid+1, end, pos, value);
        lcmAttackTree[idx] = lcm(lcmAttackTree[2*idx+1], lcmAttackTree[2*idx+2]);
    }
    void updateLCMHealth(int idx, int start, int end, int pos, int value)
    {
        if (start == end) { lcmHealthTree[idx] = value; return; }
        int mid = (start + end) / 2;
        if (pos <= mid) updateLCMHealth(2*idx+1, start, mid, pos, value);
        else            updateLCMHealth(2*idx+2, mid+1, end, pos, value);
        lcmHealthTree[idx] = lcm(lcmHealthTree[2*idx+1], lcmHealthTree[2*idx+2]);
    }

public:
    SegmentTree(const vector<pair<int,int>> &values) : a(values)
    {
        int n = values.size();
        sumAttackTree.resize(4*n);   sumHealthTree.resize(4*n);
        maxAttackTreeIndex.resize(4*n); maxHealthTreeIndex.resize(4*n);
        minAttackTreeIndex.resize(4*n); minHealthTreeIndex.resize(4*n);
        gcdAttackTree.resize(4*n);   gcdHealthTree.resize(4*n);
        lcmAttackTree.resize(4*n);   lcmHealthTree.resize(4*n);
        buildSumAttack(1,0,n-1);  buildSumHealth(1,0,n-1);
        buildMaxAttack(1,0,n-1);  buildMaxHealth(1,0,n-1);
        buildMinAttack(1,0,n-1);  buildMinHealth(1,0,n-1);
        buildGcdAttack(1,0,n-1);  buildGcdHealth(1,0,n-1);
        buildLcmAttack(1,0,n-1);  buildLcmHealth(1,0,n-1);
    }
    long long querySumAttack(int l, int r)  { return querySumAttack(1, 0, a.size()-1, l, r); }
    long long querySumHealth(int l, int r)  { return querySumHealth(1, 0, a.size()-1, l, r); }
    int queryMaxAttackIndex(int l, int r)   { return queryMaxAttackIndex(1, 0, a.size()-1, l, r); }
    int queryMaxHealthIndex(int l, int r)   { return queryMaxHealthIndex(1, 0, a.size()-1, l, r); }
    int queryMinAttackIndex(int l, int r)   { return queryMinAttackIndex(1, 0, a.size()-1, l, r); }
    int queryMinHealthIndex(int l, int r)   { return queryMinHealthIndex(1, 0, a.size()-1, l, r); }
    int queryGcdAttack(int l, int r)        { return queryGcdAttack(1, 0, a.size()-1, l, r); }
    int queryGcdHealth(int l, int r)        { return queryGcdHealth(1, 0, a.size()-1, l, r); }
    long long queryLcmAttack(int l, int r)  { return queryLcmAttack(1, 0, a.size()-1, l, r); }
    long long queryLcmHealth(int l, int r)  { return queryLcmHealth(1, 0, a.size()-1, l, r); }
    pair<int,int> getElement(int i)         { return a[i]; }
    void update(int pos, int attack, int health)
    {
        a[pos] = {attack, health};
        updateSumAttack(1,0,a.size()-1,pos,attack);
        updateSumHealth(1,0,a.size()-1,pos,health);
        updateMaxAttackIndex(pos,attack,0,a.size()-1,1);
        updateMaxHealthIndex(pos,health,0,a.size()-1,1);
        updateMinAttackIndex(pos,attack,0,a.size()-1,1);
        updateMinHealthIndex(pos,health,0,a.size()-1,1);
        updateGCDAttack(1,0,a.size()-1,pos,attack);
        updateGCDHealth(1,0,a.size()-1,pos,health);
        updateLCMAttack(1,0,a.size()-1,pos,attack);
        updateLCMHealth(1,0,a.size()-1,pos,health);
    }
};

// ===================== HELPER FUNCTIONS =====================

void pressEnterToContinue()
{
    cout << "\n  [ Press ENTER to continue... ]";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printLeaderboard(int roundNum, int scoreA, int scoreB)
{
    cout << endl;
    cout << "  ========== LEADERBOARD (After Round " << roundNum << ") ==========" << endl;
    cout << "  Team A : " << scoreA << " points"                                    << endl;
    cout << "  Team B : " << scoreB << " points"                                    << endl;
    if      (scoreA > scoreB) cout << "  Leader : TEAM A is leading by " << (scoreA-scoreB) << endl;
    else if (scoreB > scoreA) cout << "  Leader : TEAM B is leading by " << (scoreB-scoreA) << endl;
    else                      cout << "  Leader : TIED!"                                      << endl;
    cout << "  ======================================================" << endl;
}

long long applyPowerUp(int choice, long long value, bool isAttack)
{
    if (isAttack)
    {
        if      (choice == 1) { cout << "  >> Double Attack activated! Attack x2"  << endl; return value * 2; }
        else if (choice == 3) { cout << "  >> Heal activated! +500 to your attack" << endl; return value + 500; }
    }
    else
    {
        if      (choice == 2) { cout << "  >> Shield activated! Enemy health -20%" << endl; return (long long)(value * 0.8); }
        else if (choice == 3) { cout << "  >> Heal activated! +500 to your score"  << endl; return value + 500; }
    }
    return value;
}

void showScoutReport(SegmentTree &myTree, SegmentTree &enemyTree, int totalSoldiers)
{
    cout << endl;
    cout << "  ============= SCOUT REPORT =============" << endl;
    int myStrongest    = myTree.queryMaxAttackIndex(0, totalSoldiers-1);
    int myWeakest      = myTree.queryMinAttackIndex(0, totalSoldiers-1);
    int enemyWeakest   = enemyTree.queryMinHealthIndex(0, totalSoldiers-1);
    int enemyStrongest = enemyTree.queryMaxHealthIndex(0, totalSoldiers-1);
    cout << "  Your  strongest attacker : index " << myStrongest
         << "  (attack = " << myTree.getElement(myStrongest).first << ")"      << endl;
    cout << "  Your  weakest   attacker : index " << myWeakest
         << "  (attack = " << myTree.getElement(myWeakest).first << ")"        << endl;
    cout << "  Enemy weakest   defender : index " << enemyWeakest
         << "  (health = " << enemyTree.getElement(enemyWeakest).second << ")" << endl;
    cout << "  Enemy strongest defender : index " << enemyStrongest
         << "  (health = " << enemyTree.getElement(enemyStrongest).second << ")" << endl;
    cout << "  ========================================" << endl << endl;
}

// ===================== AUTO RANGE CHOOSER =====================
// Picks the starting index whose window of size soldiersPerRound
// has the highest total attack for the attacker
int autoChooseBestRange(SegmentTree &myTree, int totalSoldiers, int soldiersPerRound)
{
    long long bestSum = -1;
    int bestL = 0;
    // Sample every soldiersPerRound-th start to keep it fast
    for (int l = 0; l + soldiersPerRound - 1 < totalSoldiers; l += soldiersPerRound)
    {
        long long s = myTree.querySumAttack(l, l + soldiersPerRound - 1);
        if (s > bestSum) { bestSum = s; bestL = l; }
    }
    return bestL;
}
int autoChooseBestRange(SegmentTree &myTree, int totalSoldiers, int soldiersPerRound, vector<bool> &usedIndices)
{
    long long bestSum = -1;
    int bestL = 0;
    int step = max(1, (totalSoldiers - soldiersPerRound) / 1000);

    for (int start = 0; start <= totalSoldiers - soldiersPerRound; start += step)
    {
        // Check if this window has already been used
        bool alreadyUsed = false;
        for (int i = start; i <= start + soldiersPerRound - 1; i++)
        {
            if (usedIndices[i])
            {
                alreadyUsed = true;
                break;
            }
        }
        if (alreadyUsed) continue;

        long long s = myTree.querySumAttack(start, start + soldiersPerRound - 1);
        if (s > bestSum)
        {
            bestSum = s;
            bestL   = start;
        }
    }

    // Always check last valid window too
    int lastL = totalSoldiers - soldiersPerRound;
    bool lastUsed = false;
    for (int i = lastL; i <= lastL + soldiersPerRound - 1; i++)
    {
        if (usedIndices[i]) { lastUsed = true; break; }
    }
    if (!lastUsed)
    {
        long long lastS = myTree.querySumAttack(lastL, lastL + soldiersPerRound - 1);
        if (lastS > bestSum) bestL = lastL;
    }

    // Mark this window as used
    for (int i = bestL; i <= bestL + soldiersPerRound - 1; i++)
        usedIndices[i] = true;

    cout << "  [AUTO] Chose soldiers index " << bestL
         << " to " << (bestL + soldiersPerRound - 1)
         << " with total attack = "
         << myTree.querySumAttack(bestL, bestL + soldiersPerRound - 1) << endl;

    return bestL;
}

// ===================== PLAY ONE TEAM'S ROUNDS =====================
void playRounds(
    const string         &teamName,
    const string         &enemyName,
    SegmentTree          &myTree,
    SegmentTree          &enemyTree,
    vector<pair<int,int>> &myData,
    int  rounds,
    int  soldiersPerRound,
    int  totalSoldiers,
    int  &myScore,
    int  &enemyScore,
    int  difficulty,
    bool autoMode,
    int  &myWins,
    int  &enemyWins,
    int  &ties,
    int  &biggestMargin,
    int  &bestRound,
    long long &bestRoundScore,
    vector<int> &roundScores,
    vector<int> &rangeLeftLog,
    vector<int> &rangeRightLog
)
{
    cout << endl;
    cout << "**************************************************"    << endl;
    cout << "  " << teamName << " TURN  -  " << rounds << " ROUNDS" << endl;
    if (autoMode) cout << "  [AUTO MODE   - computer chooses ranges]"  << endl;
    else          cout << "  [MANUAL MODE - you choose any range]"      << endl;
    cout << "**************************************************"    << endl;

    // Only needed for auto mode
    vector<bool> usedIndices(totalSoldiers, false);

    for (int round = 1; round <= rounds; round++)
    {
        cout << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "  " << teamName << "  -  ROUND " << round           << endl;
        cout << "--------------------------------------------------" << endl;

        int l = 0, r = 0;

        // ---- Range Selection ----
        if (autoMode)
        {
            // Auto: pick best unused window
            l = autoChooseBestRange(myTree, totalSoldiers, soldiersPerRound, usedIndices);
            r = l + soldiersPerRound - 1;
        }
        else
        {
            // Scout report option
            char scout;
            cout << "  Do you want a Scout Report? (y/n): ";
            cin >> scout;
            if (scout == 'y' || scout == 'Y')
                showScoutReport(myTree, enemyTree, totalSoldiers);

            // FREE RANGE CHOICE - user enters any l and r
            bool validRange = false;
            while (!validRange)
            {
                cout << "  Enter start index (l) and end index (r)"      << endl;
                cout << "  Valid range: 0 to " << (totalSoldiers - 1)    << endl;
                cout << "  Note: range size must be exactly "
                     << soldiersPerRound << " soldiers"                   << endl;
                cout << "  Enter l: ";
                cin >> l;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "  ERROR: Invalid input. Try again." << endl;
                    continue;
                }
                cout << "  Enter r: ";
                cin >> r;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "  ERROR: Invalid input. Try again." << endl;
                    continue;
                }

                // Validate bounds
                if (l < 0 || r >= totalSoldiers)
                {
                    cout << "  ERROR: Out of bounds!"
                         << " Must be between 0 and " << (totalSoldiers-1)
                         << ". Try again." << endl;
                    continue;
                }

                // Validate l <= r
                if (l > r)
                {
                    cout << "  ERROR: l must be <= r. Try again." << endl;
                    continue;
                }

                // Validate exact size
                if ((r - l + 1) != soldiersPerRound)
                {
                    cout << "  ERROR: Range size must be exactly "
                         << soldiersPerRound
                         << " soldiers. You entered " << (r - l + 1)
                         << ". Try again." << endl;
                    continue;
                }

                validRange = true;
            }

            cout << "  Attacking with soldiers index " << l
                 << " to " << r
                 << " (size = " << (r - l + 1) << ")" << endl;
        }

        rangeLeftLog.push_back(l);
        rangeRightLog.push_back(r);

        // ---- Power Up (manual mode only) ----
        int powerUp = 4;
        if (!autoMode)
        {
            cout << endl;
            cout << "  Choose your power-up:"                             << endl;
            cout << "    1. Double Attack  (your attack x2)"              << endl;
            cout << "    2. Shield         (enemy health -20%)"           << endl;
            cout << "    3. Heal           (+500 to your attack)"         << endl;
            cout << "    4. No power-up"                                  << endl;
            cout << "  Enter choice (1-4): ";
            cin >> powerUp;
            if (cin.fail() || powerUp < 1 || powerUp > 4)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                powerUp = 4;
            }
        }

        long long myAttack    = myTree.querySumAttack(l, r);
        long long enemyHealth = enemyTree.querySumHealth(l, r);

        // Apply difficulty
        if (difficulty == 1) myAttack    = (long long)(myAttack    * 1.5);
        if (difficulty == 3) enemyHealth = (long long)(enemyHealth * 1.5);

        // Apply power-up
        myAttack    = applyPowerUp(powerUp, myAttack,    true);
        enemyHealth = applyPowerUp(powerUp, enemyHealth, false);

        long long roundScore = myAttack - enemyHealth;
        myScore += (int)roundScore;

        // Track stats
        if      (roundScore > 0) { myWins++;    if (roundScore > biggestMargin) { biggestMargin = (int)roundScore; bestRound = round; bestRoundScore = roundScore; } }
        else if (roundScore < 0)   enemyWins++;
        else                       ties++;

        // Boost attacked soldiers attack to 100
        for (int i = l; i <= r; i++)
        {
            myData[i].first = 100;
            myTree.update(i, 100, myData[i].second);
        }

        roundScores.push_back((int)roundScore);

        // ---- Print Round Result ----
        cout << endl;
        cout << "  ROUND " << round << " RESULT:"                                          << endl;
        cout << "  Soldiers Range  : index " << l << " to " << r                           << endl;
        cout << "  " << teamName << " Attack   : " << myAttack                             << endl;
        cout << "  " << enemyName << " Health   : " << enemyHealth                         << endl;
        cout << "  Round Score     : " << roundScore                                        << endl;
        if      (roundScore > 0) cout << "  >>> " << teamName << " WINS round "  << round << "! (+" << roundScore << ")" << endl;
        else if (roundScore == 0) cout << "  >>> Round " << round << " is a TIE!"           << endl;
        else                     cout << "  >>> " << teamName << " LOSES round " << round << "! (" << roundScore << ")"  << endl;

        // ---- LCM Surprise every 10th round ----
        if (round % 10 == 0)
        {
            cout << endl;
            cout << "  *** SURPRISE LCM ROUND! ***"                                   << endl;
            long long lcm1 = myTree.queryLcmAttack(l, r);
            long long lcm2 = enemyTree.queryLcmHealth(l, r);
            cout << "  " << teamName  << " LCM of Attacks : " << lcm1                << endl;
            cout << "  " << enemyName << " LCM of Health  : " << lcm2                << endl;
            if (lcm1 > lcm2) { myScore += 50; cout << "  >>> " << teamName << " wins LCM round! +50 bonus!" << endl; }
            else                               cout << "  >>> " << teamName << " loses LCM round. No bonus." << endl;
        }

        // ---- GCD Surprise every 11th round ----
        if (round % 11 == 0)
        {
            cout << endl;
            cout << "  *** SURPRISE GCD ROUND! ***"                                   << endl;
            long long gcd1 = myTree.queryGcdAttack(l, r);
            long long gcd2 = enemyTree.queryGcdHealth(l, r);
            cout << "  " << teamName  << " GCD of Attacks : " << gcd1                << endl;
            cout << "  " << enemyName << " GCD of Health  : " << gcd2                << endl;
            if (gcd1 > gcd2) { myScore += 50; cout << "  >>> " << teamName << " wins GCD round! +50 bonus!" << endl; }
            else                               cout << "  >>> " << teamName << " loses GCD round. No bonus." << endl;
        }

        cout << "  " << teamName << " Total Score so far : " << myScore << endl;

        // Leaderboard every 10 rounds
        if (round % 10 == 0)
            printLeaderboard(round, myScore, enemyScore);
    }

    cout << endl;
    cout << "  " << teamName << " has finished all " << rounds << " rounds!" << endl;
    cout << "  " << teamName << " Final Score : " << myScore                 << endl;
}

// ===================== PRINT END SUMMARY =====================
void printEndSummary(
    const string        &teamName,
    int                  rounds,
    int                  soldiersPerRound,
    const vector<int>   &roundScores,
    const vector<int>   &rangeLeft,
    const vector<int>   &rangeRight,
    int                  totalWins,
    int                  totalLosses,
    int                  totalTies,
    int                  bestRound,
    long long            bestRoundScore,
    int                  biggestMargin
)
{
    cout << endl;
    cout << "==================================================" << endl;
    cout << "        END SUMMARY FOR " << teamName               << endl;
    cout << "==================================================" << endl;
    cout << "  Rounds Played    : " << rounds                    << endl;
    cout << "  Soldiers/Round   : " << soldiersPerRound          << endl;
    cout << "  Rounds Won       : " << totalWins                 << endl;
    cout << "  Rounds Lost      : " << totalLosses               << endl;
    cout << "  Rounds Tied      : " << totalTies                 << endl;
    cout << "  Best Round       : Round " << bestRound
         << " (score = " << bestRoundScore << ")"                << endl;
    cout << "  Biggest Margin   : " << biggestMargin             << endl;
    cout << "--------------------------------------------------"  << endl;
    cout << "  ROUND-BY-ROUND BREAKDOWN:"                        << endl;
    cout << "--------------------------------------------------"  << endl;

    for (int i = 0; i < (int)roundScores.size(); i++)
    {
        cout << "  Round " << (i+1)
             << "  | Range: " << rangeLeft[i] << "-" << rangeRight[i]
             << "  | Score: " << roundScores[i];
        if      (roundScores[i] > 0) cout << "  [WIN]";
        else if (roundScores[i] < 0) cout << "  [LOSS]";
        else                         cout << "  [TIE]";
        cout << endl;
    }
    cout << "==================================================" << endl;
}

// ===================== MAIN =====================
int main()
{
    const int numSoldiers = 100000;
    vector<pair<int,int>> team1(numSoldiers);
    vector<pair<int,int>> team2(numSoldiers);

    ifstream infile1("team1.txt");
    if (!infile1.is_open()) { cerr << "Error opening team1.txt!" << endl; return 1; }
    ifstream infile2("team2.txt");
    if (!infile2.is_open()) { cerr << "Error opening team2.txt!" << endl; return 1; }
    for (int i = 0; i < numSoldiers; i++) { int a,h; infile1>>a>>h; team1[i]={a,h}; }
    for (int i = 0; i < numSoldiers; i++) { int a,h; infile2>>a>>h; team2[i]={a,h}; }
    infile1.close();
    infile2.close();

    char playAgain = 'y';

    while (playAgain == 'y' || playAgain == 'Y')
    {
        // Rebuild trees fresh for each game
        vector<pair<int,int>> t1 = team1;
        vector<pair<int,int>> t2 = team2;
        SegmentTree segtree1(t1);
        SegmentTree segtree2(t2);

        cout << "==================================================" << endl;
        cout << "         WELCOME TO THE BATTLE GAME!             " << endl;
        cout << "==================================================" << endl;
        cout << "GAME RULES:"                                        << endl;
        cout << "  -> Each team has 100,000 soldiers"               << endl;
        cout << "  -> Manual: you pick soldiers each round"         << endl;
        cout << "  -> Auto  : computer picks best range"            << endl;
        cout << "  -> Score = Your Attack Sum - Enemy Health Sum"   << endl;
        cout << "  -> Every 10th round = LCM Surprise (+50 bonus)"  << endl;
        cout << "  -> Every 11th round = GCD Surprise (+50 bonus)"  << endl;
        cout << "==================================================" << endl;
        cout << endl;

        // ---- Difficulty ----
        int difficulty;
        cout << "Select difficulty:"                   << endl;
        cout << "  1. Easy   (your attack x1.5)"      << endl;
        cout << "  2. Normal (no bonus)"               << endl;
        cout << "  3. Hard   (enemy health x1.5)"     << endl;
        cout << "Enter choice (1-3): ";
        cin >> difficulty;
        if (cin.fail() || difficulty < 1 || difficulty > 3)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            difficulty = 2;
        }

        // ---- Rounds ----
        int rounds;
        cout << endl;
        cout << "Enter number of rounds (1 to 100000): ";
        cin >> rounds;
        while (cin.fail() || rounds <= 0 || rounds > 100000)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  ERROR: Enter a number between 1 and 100000: ";
            cin >> rounds;
        }

        // Soldiers per round is suggested but user can enter any range
        int soldiersPerRound = 100000 / rounds;
        if (soldiersPerRound < 1) soldiersPerRound = 1;

        cout << "  Suggested soldiers per round : " << soldiersPerRound << endl;
        cout << "  (You can still enter any range of exactly this size)" << endl;


        // ---- Mode for Team A ----
        int modeA;
        cout << endl;
        cout << "Select mode for TEAM A:"                               << endl;
        cout << "  1. Manual - you choose attack range each round"      << endl;
        cout << "  2. Auto   - computer picks best range automatically" << endl;
        cout << "Enter choice (1 or 2): ";
        cin >> modeA;
        if (cin.fail() || (modeA != 1 && modeA != 2))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            modeA = 2;
        }
        bool autoModeA = (modeA == 2);

        // ---- Mode for Team B ----
        int modeB;
        cout << endl;
        cout << "Select mode for TEAM B:"                               << endl;
        cout << "  1. Manual - you choose attack range each round"      << endl;
        cout << "  2. Auto   - computer picks best range automatically" << endl;
        cout << "Enter choice (1 or 2): ";
        cin >> modeB;
        if (cin.fail() || (modeB != 1 && modeB != 2))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            modeB = 2;
        }
        bool autoModeB = (modeB == 2);

        // Clear leftover newline before rounds start
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << endl;
        cout << "==================================================" << endl;
        cout << "  Total Soldiers  : 100000"                         << endl;
        cout << "  Rounds          : " << rounds                     << endl;
        cout << "  Soldiers/Round  : " << soldiersPerRound           << endl;
        cout << "  Difficulty      : " << (difficulty==1?"Easy":difficulty==2?"Normal":"Hard") << endl;
        cout << "  Team A Mode     : " << (autoModeA ? "Auto" : "Manual") << endl;
        cout << "  Team B Mode     : " << (autoModeB ? "Auto" : "Manual") << endl;
        cout << "==================================================" << endl;

        int teamAscore = 0, teamBscore = 0;
        int teamAWins = 0, teamALosses = 0, teamATies = 0;
        int teamBWins = 0, teamBLosses = 0, teamBTies = 0;
        int biggestMarginA = 0, biggestMarginB = 0;
        int bestRoundA = 0, bestRoundB = 0;
        long long bestRoundScoreA = 0, bestRoundScoreB = 0;

        vector<int> roundScoresA, roundScoresB;
        vector<int> rangeLeftA,   rangeRightA;
        vector<int> rangeLeftB,   rangeRightB;

        // ---- Team A Plays ----
        playRounds(
            "TEAM A", "TEAM B",
            segtree1, segtree2, t1,
            rounds, soldiersPerRound, numSoldiers,
            teamAscore, teamBscore,
            difficulty, autoModeA,
            teamAWins, teamALosses, teamATies,
            biggestMarginA, bestRoundA, bestRoundScoreA,
            roundScoresA, rangeLeftA, rangeRightA
        );

        cout << endl;
        cout << "==================================================" << endl;
        cout << "  TEAM A DONE! Score = " << teamAscore              << endl;
        cout << "  Now TEAM B will play..."                          << endl;
        cout << "==================================================" << endl;

        // ---- Team B Plays ----
        playRounds(
            "TEAM B", "TEAM A",
            segtree2, segtree1, t2,
            rounds, soldiersPerRound, numSoldiers,
            teamBscore, teamAscore,
            difficulty, autoModeB,
            teamBWins, teamBLosses, teamBTies,
            biggestMarginB, bestRoundB, bestRoundScoreB,
            roundScoresB, rangeLeftB, rangeRightB
        );

        // ---- End Summary ----
        printEndSummary(
            "TEAM A", rounds, soldiersPerRound,
            roundScoresA, rangeLeftA, rangeRightA,
            teamAWins, teamALosses, teamATies,
            bestRoundA, bestRoundScoreA, biggestMarginA
        );

        printEndSummary(
            "TEAM B", rounds, soldiersPerRound,
            roundScoresB, rangeLeftB, rangeRightB,
            teamBWins, teamBLosses, teamBTies,
            bestRoundB, bestRoundScoreB, biggestMarginB
        );

        // ---- Final Result ----
        cout << endl;
        cout << "==================================================" << endl;
        cout << "               FINAL RESULT                      " << endl;
        cout << "==================================================" << endl;
        cout << "  Team A Final Score : " << teamAscore              << endl;
        cout << "  Team B Final Score : " << teamBscore              << endl;
        cout << "--------------------------------------------------"  << endl;
        if (teamAscore > teamBscore)
        {
            cout << "  >>> TEAM A WON THE GAME! CONGRATULATIONS!"        << endl;
            cout << "  >>> Winning margin : " << (teamAscore - teamBscore) << endl;
        }
        else if (teamAscore == teamBscore)
            cout << "  >>> THE MATCH IS TIED! WELL PLAYED BOTH!"         << endl;
        else
        {
            cout << "  >>> TEAM B WON THE GAME! CONGRATULATIONS!"        << endl;
            cout << "  >>> Winning margin : " << (teamBscore - teamAscore) << endl;
        }
        cout << "==================================================" << endl;

        cout << endl;
        cout << "  Do you want a rematch? (y/n): ";
        cin >> playAgain;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << endl;
    cout << "  Thanks for playing! Goodbye!"     << endl;
    cout << "==================================================" << endl;
    return 0;
}
