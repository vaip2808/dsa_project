#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <utility>
#include <numeric>
#include <tuple>

using namespace std;

class SegmentTree
{
private:
    vector<pair<int, int>> a; // Vector of pairs (attack, health)
    vector<long long> sumAttackTree, sumHealthTree; // Segment trees for sum of attacks and healths
    vector<int> maxAttackTreeIndex, maxHealthTreeIndex; // Segment trees for max attack and health indices
    vector<int> minAttackTreeIndex, minHealthTreeIndex; // Segment trees for min attack and health indices
    vector<int> gcdAttackTree, gcdHealthTree; // Segment trees for GCD of attacks and healths
    vector<long long> lcmAttackTree, lcmHealthTree; // Segment trees for LCM of attacks and healths

    void buildSumAttack(int node, int start, int end) // Function to build the sum attack segment tree
    {
        if (start == end) 
        {
            sumAttackTree[node] = a[start].first; // Leaf node
            return;
        }
        int mid = (start + end) / 2;
        buildSumAttack(2 * node + 1, start, mid); // Build left child
        buildSumAttack(2 * node + 2, mid + 1, end); // Build right child
        sumAttackTree[node] = sumAttackTree[2 * node + 1] + sumAttackTree[2 * node + 2]; // Combine results
    }

    void buildSumHealth(int node, int start, int end) // Function to build the sum health segment tree
    {
        if (start == end) 
        {
            sumHealthTree[node] = a[start].second; // Leaf node
            return;
        }
        int mid = (start + end) / 2;
        buildSumHealth(2 * node + 1, start, mid); // Build left child
        buildSumHealth(2 * node + 2, mid + 1, end); // Build right child
        sumHealthTree[node] = sumHealthTree[2 * node + 1] + sumHealthTree[2 * node + 2]; // Combine results
    }

    int querySumAttack(int node, int start, int end, int l, int r) // Function to query the sum of attacks in a given range
    {
        if (start > r || end < l) 
        {
            return 0; // Out of range
        }
        if (l <= start && r >= end)
        {
            return sumAttackTree[node]; // Fully in range
        }
        int mid = (start + end) / 2;
        int leftSum = querySumAttack(2 * node + 1, start, mid, l, r); // Query left child
        int rightSum = querySumAttack(2 * node + 2, mid + 1, end, l, r); // Query right child
        return leftSum + rightSum; // Combine results
    }

    long long lcm(int a, int b) // Function to calculate the LCM of two numbers
    {
        if (a == 0 || b == 0) 
            return 0;                            // LCM of 0 with any number is 0
        return (long long)a * (b / __gcd(a, b)); // Calculate LCM
    }

    int querySumHealth(int node, int start, int end, int l, int r) 
        // Function to query the sum of healths in a given range
       {
        if (start > r || end < l)
        {
            return 0; // Out of range
        }
        if (l <= start && r >= end)
        {
            return sumHealthTree[node]; // Fully in range
        }
        int mid = (start + end) / 2;
        int leftSum = querySumHealth(2 * node + 1, start, mid, l, r); // Query left child
        int rightSum = querySumHealth(2 * node + 2, mid + 1, end, l, r); // Query right child
        return leftSum + rightSum; // Combine results
    }
    void buildMaxAttack(int node, int start, int end) // Function to build the max attack segment tree
    {
        if (start == end)
        {
            maxAttackTreeIndex[node] = start; // Leaf node
            return;
        }
        int mid = (start + end) / 2;
        buildMaxAttack(2 * node + 1, start, mid); // Build left child
        buildMaxAttack(2 * node + 2, mid + 1, end); // Build right child
        maxAttackTreeIndex[node] = (a[maxAttackTreeIndex[2 * node + 1]].first > a[maxAttackTreeIndex[2 * node + 2]].first)
                                       ? maxAttackTreeIndex[2 * node + 1]
                                       : maxAttackTreeIndex[2 * node + 2]; // Combine results
    }
    
    void buildMaxHealth(int node, int start, int end) // Function to build the max health segment tree
    {
        if (start == end) 
        {
            maxHealthTreeIndex[node] = start; // Leaf node
            return;
        }
        int mid = (start + end) / 2; // Calculate the mid point
        buildMaxHealth(2 * node + 1, start, mid); // Build left child
        buildMaxHealth(2 * node + 2, mid + 1, end); // Build right child
        maxHealthTreeIndex[node] = (a[maxHealthTreeIndex[2 * node + 1]].second > a[maxHealthTreeIndex[2 * node + 2]].second) // Combine results
                                       ? maxHealthTreeIndex[2 * node + 1] 
                                       : maxHealthTreeIndex[2 * node + 2];
    }

    void buildMinAttack(int node, int start, int end)  // Function to build the min attack segment tree
    {
        if (start == end) 
        {
            minAttackTreeIndex[node] = start; // Leaf node
            return;
        }
        int mid = (start + end) / 2; // Calculate the mid point
        buildMinAttack(2 * node + 1, start, mid); // Build left child
        buildMinAttack(2 * node + 2, mid + 1, end); // Build right child
        minAttackTreeIndex[node] = (a[minAttackTreeIndex[2 * node + 1]].first < a[minAttackTreeIndex[2 * node + 2]].first) // Combine results
                                       ? minAttackTreeIndex[2 * node + 1]
                                       : minAttackTreeIndex[2 * node + 2];
    }

    void buildMinHealth(int node, int start, int end) // Function to build the min health segment tree
    {
        if (start == end) 
        {
            minHealthTreeIndex[node] = start; // Leaf node
            return;
        }
        int mid = (start + end) / 2; // Calculate the mid point
        buildMinHealth(2 * node + 1, start, mid); // Build left child
        buildMinHealth(2 * node + 2, mid + 1, end); // Build right child
        minHealthTreeIndex[node] = (a[minHealthTreeIndex[2 * node + 1]].second < a[minHealthTreeIndex[2 * node + 2]].second) // Combine results
                                       ? minHealthTreeIndex[2 * node + 1]
                                       : minHealthTreeIndex[2 * node + 2];
    }

    int queryMaxAttackIndex(int node, int start, int end, int l, int r) // Function to query the index of max attack in a given range
    {
        if (start > r || end < l) 
        {
            return -1; // Out of range
        }
        if (l <= start && r >= end) 
        {
            return maxAttackTreeIndex[node]; // Fully in range
        }
        int mid = (start + end) / 2;
        int leftIndex = queryMaxAttackIndex(2 * node + 1, start, mid, l, r); // Query left child
        int rightIndex = queryMaxAttackIndex(2 * node + 2, mid + 1, end, l, r); // Query right child
        if (leftIndex == -1) 
        {
            return rightIndex;
        }
        if (rightIndex == -1) 
        {
            return leftIndex;
        }
        return (a[leftIndex].first > a[rightIndex].first) ? leftIndex : rightIndex; // Combine results
    }

    int queryMaxHealthIndex(int node, int start, int end, int l, int r) // Function to query the index of max health in a given range
    {
        if (start > r || end < l) 
        {
            return -1; // Out of range
        }
        if (l <= start && r >= end) 
        {
            return maxHealthTreeIndex[node]; // Fully in range
        }
        int mid = (start + end) / 2; // Calculate the mid point
        int leftIndex = queryMaxHealthIndex(2 * node + 1, start, mid, l, r); // Query left child
        int rightIndex = queryMaxHealthIndex(2 * node + 2, mid + 1, end, l, r); // Query right child
        if (leftIndex == -1) 
        {
            return rightIndex;
        }
        if (rightIndex == -1) 
        {
            return leftIndex;
        }
        return (a[leftIndex].second > a[rightIndex].second) ? leftIndex : rightIndex; // Combine results
    }

    int queryMinAttackIndex(int node, int start, int end, int l, int r) // Function to query the index of min attack in a given range
    {
        if (start > r || end < l)
            return -1; // Out of range
        if (l <= start && r >= end)
            return minAttackTreeIndex[node]; // Fully in range
        int mid = (start + end) / 2; // Calculate the mid point
        int leftIndex = queryMinAttackIndex(2 * node + 1, start, mid, l, r); // Query left child
        int rightIndex = queryMinAttackIndex(2 * node + 2, mid + 1, end, l, r); // Query right child
        if (leftIndex == -1)  // Combine results
            return rightIndex;
        if (rightIndex == -1)  // Combine results
            return leftIndex;
        return (a[leftIndex].first < a[rightIndex].first) ? leftIndex : rightIndex; // Combine results
    }

    int queryMinHealthIndex(int node, int start, int end, int l, int r) // Function to query the index of min health in a given range
    {
        if (start > r || end < l)
            return -1; // Out of range
        if (l <= start && r >= end)
            return minHealthTreeIndex[node]; // Fully in range
        int mid = (start + end) / 2;
        int leftIndex = queryMinHealthIndex(2 * node + 1, start, mid, l, r); // Query left child
        int rightIndex = queryMinHealthIndex(2 * node + 2, mid + 1, end, l, r); // Query right child
        if (leftIndex == -1)
            return rightIndex;
        if (rightIndex == -1)
            return leftIndex;
        return (a[leftIndex].second < a[rightIndex].second) ? leftIndex : rightIndex; // Combine results
    }

    void buildGcdAttack(int node, int start, int end) // Function to build the GCD attack segment tree
    {
        if (start == end)
        {
            gcdAttackTree[node] = a[start].first; // Leaf node
            return;
        }
        int mid = (start + end) / 2; // Calculate the mid point
        buildGcdAttack(2 * node + 1, start, mid); // Build left child
        buildGcdAttack(2 * node + 2, mid + 1, end);     // Build right child
        gcdAttackTree[node] = __gcd(gcdAttackTree[2 * node + 1], gcdAttackTree[2 * node + 2]); // Combine results
    }

    void buildGcdHealth(int node, int start, int end) // Function to build the GCD health segment tree
    {
        if (start == end)
        {
            gcdHealthTree[node] = a[start].second; // Leaf node
            return;
        }
        int mid = (start + end) / 2; // Calculate the mid point
        buildGcdHealth(2 * node + 1, start, mid); // Build left child
        buildGcdHealth(2 * node + 2, mid + 1, end); // Build right child
        gcdHealthTree[node] = __gcd(gcdHealthTree[2 * node + 1], gcdHealthTree[2 * node + 2]); // Combine results
    }

    void buildLcmAttack(int node, int start, int end) // Function to build the LCM attack segment tree
    {
        if (start == end)
        {
            lcmAttackTree[node] = a[start].first; // Leaf node
            return;
        }
        int mid = (start + end) / 2; // Calculate the mid point
        buildLcmAttack(2 * node + 1, start, mid); // Build left child
        buildLcmAttack(2 * node + 2, mid + 1, end);     // Build right child
        lcmAttackTree[node] = lcm(lcmAttackTree[2 * node + 1], lcmAttackTree[2 * node + 2]); // Combine results
    }

    void buildLcmHealth(int node, int start, int end)  // Function to build the LCM health segment tree
    {
        if (start == end)
        {
            lcmHealthTree[node] = a[start].second; // Leaf node
            return;
        }
        int mid = (start + end) / 2; // Calculate the mid point
        buildLcmHealth(2 * node + 1, start, mid); // Build left child
        buildLcmHealth(2 * node + 2, mid + 1, end); // Build right child
        lcmHealthTree[node] = lcm(lcmHealthTree[2 * node + 1], lcmHealthTree[2 * node + 2]); // Combine results
    }

    int queryGcdAttack(int node, int start, int end, int l, int r)  // Function to query the GCD of attacks in a given range
    {
        if (start > r || end < l)
        {
            return 0; // Out of range
        }
        if (l <= start && r >= end)
        {
            return gcdAttackTree[node]; // Fully in range
        }
        int mid = (start + end) / 2; // Calculate the mid point
        int leftGcd = queryGcdAttack(2 * node + 1, start, mid, l, r); // Query left child
        int rightGcd = queryGcdAttack(2 * node + 2, mid + 1, end, l, r);  // Query right child
        return __gcd(leftGcd, rightGcd); // Combine results
    }

    int queryGcdHealth(int node, int start, int end, int l, int r)  // Function to query the GCD of healths in a given range
    {
        if (start > r || end < l)
        {
            return 0; // Out of range
        }
        if (l <= start && r >= end)
        {
            return gcdHealthTree[node]; // Fully in range
        }
        int mid = (start + end) / 2; // Calculate the mid point
        int leftGcd = queryGcdHealth(2 * node + 1, start, mid, l, r); // Query left child
        int rightGcd = queryGcdHealth(2 * node + 2, mid + 1, end, l, r); // Query right child
        return __gcd(leftGcd, rightGcd); // Combine results
    }
    
    int queryLcmAttack(int node, int start, int end, int l, int r)  // Function to query the LCM of attacks in a given range
    {
        if (start > r || end < l)
        {
            return 1; // LCM of 1 is neutral
        }
        if (l <= start && r >= end)
        {
            return lcmAttackTree[node]; // Fully in range
        }
        int mid = (start + end) / 2; // Calculate the mid point
        int leftLcm = queryLcmAttack(2 * node + 1, start, mid, l, r); // Query left child
        int rightLcm = queryLcmAttack(2 * node + 2, mid + 1, end, l, r); // Query right child
        return lcm(leftLcm, rightLcm); // Combine results
    }
    
    int queryLcmHealth(int node, int start, int end, int l, int r)  // Function to query the LCM of healths in a given range
    {
        if (start > r || end < l)
        {
            return 1; // LCM of 1 is neutral
        }
        if (l <= start && r >= end)
        {
            return lcmHealthTree[node]; // Fully in range
        }
        int mid = (start + end) / 2; // Calculate the mid point
        int leftLcm = queryLcmHealth(2 * node + 1, start, mid, l, r); // Query left child
        int rightLcm = queryLcmHealth(2 * node + 2, mid + 1, end, l, r); // Query right child
        return lcm(leftLcm, rightLcm); // Combine results
    }

    // Update functions
    void updateSumAttack(int idx, int start, int end, int pos, int value)  // Function to update the sum of attacks at a specific position
    {
        if (start == end) // If it's a leaf node
        {
            sumAttackTree[idx] = value; // Update the value directly
        }
        else
        {
            int mid = (start + end) / 2; // Calculate the mid point
            if (pos <= mid) // If the position is in the left child
            {
                updateSumAttack(2 * idx + 1, start, mid, pos, value); // Update left child
            }
            else // If the position is in the right child
            {
                updateSumAttack(2 * idx + 2, mid + 1, end, pos, value); // Update right child
            }
            sumAttackTree[idx] = sumAttackTree[2 * idx + 1] + sumAttackTree[2 * idx + 2]; // Combine results
        }
    }

    void updateSumHealth(int idx, int start, int end, int pos, int value)  // Function to update the sum of healths at a specific position
    {
        if (start == end) // If it's a leaf node
        {
            sumHealthTree[idx] = value; // Update the value directly
        }
        else
        {
            int mid = (start + end) / 2; // Calculate the mid point
            if (pos <= mid) // If the position is in the left child
            {
                updateSumHealth(2 * idx + 1, start, mid, pos, value); // Update left child
            }
            else // If the position is in the right child
            {
                updateSumHealth(2 * idx + 2, mid + 1, end, pos, value); // Update right child
            }
            sumHealthTree[idx] = sumHealthTree[2 * idx + 1] + sumHealthTree[2 * idx + 2]; // Combine results
        }
    }

    void updateMaxAttackIndex(int index, int value, vector<pair<int, int>> &a, int start, int end, int node)  // Function to update the attack value at a specific index and adjust the maximum attack index
    {
        if (start == end)
        {
            // Leaf node, update the attack value directly          // Update the original array
            maxAttackTreeIndex[node] = start;
            return; // Update the index in the tree
        }
        else
        {
            int mid = (start + end) / 2; // Calculate the mid point
            if (index <= mid)
            {
                // Update left child
                updateMaxAttackIndex(index, value, a, start, mid, 2 * node + 1); // Left child
            }
            else
            {
                // Update right child
                updateMaxAttackIndex(index, value, a, mid + 1, end, 2 * node + 2); // Right child
            }
            // After updating, recalculate the index of maximum attack value
            if (a[maxAttackTreeIndex[2 * node + 1]].first > a[maxAttackTreeIndex[2 * node + 2]].first)
            {
                maxAttackTreeIndex[node] = maxAttackTreeIndex[2 * node + 1]; // Update to left child
            }
            else
            {
                maxAttackTreeIndex[node] = maxAttackTreeIndex[2 * node + 2]; // Update to right child
            }
        }
    }

    // Function to update the health value at a specific index and adjust the maximum health index
    void updateMaxHealthIndex(int index, int value, vector<pair<int, int>> &a, int start, int end, int node)
    {
        if (start == end)
        {
            // Leaf node, update the health value directly
            a[index].second = value; // Update the original array
            maxHealthTreeIndex[node] = index;
            return; // Update the index in the tree
        }
        else
        {
            int mid = (start + end) / 2; // Calculate the mid point
            if (index <= mid)
            {
                // Update left child
                updateMaxHealthIndex(index, value, a, start, mid, 2 * node + 1); // Left child
            }
            else
            {
                // Update right child
                updateMaxHealthIndex(index, value, a, mid + 1, end, 2 * node + 2); // Right child
            }
            // After updating, recalculate the index of maximum health value
            if (a[maxHealthTreeIndex[2 * node + 1]].second > a[maxHealthTreeIndex[2 * node + 2]].second)
            {
                maxHealthTreeIndex[node] = maxHealthTreeIndex[2 * node + 1];
            }
            else
            {
                maxHealthTreeIndex[node] = maxHealthTreeIndex[2 * node + 2];
            }
        }
    }
    // Function to update the attack value at a specific index and adjust the minimum attack index
    void updateMinAttackIndex(int index, int value, vector<pair<int, int>> &a, int start, int end, int node)
    {
        if (start == end)
        {
            // Leaf node, update the attack value directly
            a[index].first = value; // Update the original array
            minAttackTreeIndex[node] = index;
            return; // Update the index in the tree
        }
        else
        {
            int mid = (start + end) / 2;
            if (index <= mid)
            {
                // Update left child
                updateMinAttackIndex(index, value, a, start, mid, 2 * node + 1); // Left child
            }
            else
            {
                // Update right child
                updateMinAttackIndex(index, value, a, mid + 1, end, 2 * node + 2); // Right child
            }
            // After updating, recalculate the index of minimum attack value
            if (a[minAttackTreeIndex[2 * node + 1]].first < a[minAttackTreeIndex[2 * node + 2]].first)
            {
                minAttackTreeIndex[node] = minAttackTreeIndex[2 * node + 1]; // Update to left child
            }
            else
            {
                minAttackTreeIndex[node] = minAttackTreeIndex[2 * node + 2]; // Update to right child
            }
        }
    }

    // Function to update the health value at a specific index and adjust the minimum health index
    void updateMinHealthIndex(int index, int value, vector<pair<int, int>> &a, int start, int end, int node)
    {
        if (start == end)
        {
            // Leaf node, update the health value directly
            a[index].second = value; // Update the original array
            minHealthTreeIndex[node] = index;
            return; // Update the index in the tree
        }
        else
        {
            int mid = (start + end) / 2;
            if (index <= mid)
            {
                // Update left child
                updateMinHealthIndex(index, value, a, start, mid, 2 * node + 1); // Left child
            }
            else
            {
                // Update right child
                updateMinHealthIndex(index, value, a, mid + 1, end, 2 * node + 2); // Right child
            }
            // After updating, recalculate the index of minimum health value
            if (a[minHealthTreeIndex[2 * node + 1]].second < a[minHealthTreeIndex[2 * node + 2]].second)
            {
                minHealthTreeIndex[node] = minHealthTreeIndex[2 * node + 1]; // Update to left child
            }
            else
            {
                minHealthTreeIndex[node] = minHealthTreeIndex[2 * node + 2]; // Update to right child
            }
        }
    }

    void updateGCDHealth(int idx, int start, int end, int pos, int value)  // Function to update the health value at a specific index and adjust the GCD health
    {
        if (start == end)
        {
            gcdHealthTree[idx] = value;  // Leaf node
        }
        else
        {
            int mid = (start + end) / 2; // Calculate the mid point
            if (pos <= mid)
            {
                updateGCDHealth(2 * idx + 1, start, mid, pos, value); // Update left child
            }
            else
            {
                updateGCDHealth(2 * idx + 2, mid + 1, end, pos, value); // Update right child
            }
            gcdHealthTree[idx] = __gcd(gcdHealthTree[2 * idx + 1], gcdHealthTree[2 * idx + 2]); // Combine results
        }
    }

    void updateLCMHealth(int idx, int start, int end, int pos, int value) // Function to update the health value at a specific index and adjust the LCM health
    {
        if (start == end)
        {
            lcmHealthTree[idx] = value; // Leaf node
        }
        else
        {
            int mid = (start + end) / 2; // Calculate the mid point
            if (pos <= mid)
            {
                updateLCMHealth(2 * idx + 1, start, mid, pos, value); // Update left child
            }
            else
            {
                updateLCMHealth(2 * idx + 2, mid + 1, end, pos, value); // Update right child
            }
            lcmHealthTree[idx] = lcm(lcmHealthTree[2 * idx + 1], lcmHealthTree[2 * idx + 2]); // Combine results
        }
    }

    void updateGCDAttack(int idx, int start, int end, int pos, int value)  // Function to update the attack value at a specific index and adjust the GCD attack
    {
        if (start == end)
        {
            gcdAttackTree[idx] = value; // Leaf node
        }
        else
        {
            int mid = (start + end) / 2; // Calculate the mid point
            if (pos <= mid)
            {
                updateGCDAttack(2 * idx + 1, start, mid, pos, value); // Update left child
            }
            else
            {
                updateGCDAttack(2 * idx + 2, mid + 1, end, pos, value); // Update right child
            }
            gcdAttackTree[idx] = __gcd(gcdAttackTree[2 * idx + 1], gcdAttackTree[2 * idx + 2]); // Combine results
        }
    }

    void updateLCMAttack(int idx, int start, int end, int pos, int value)  // Function to update the attack value at a specific index and adjust the LCM attack
    {
        if (start == end)
        {
            lcmAttackTree[idx] = value; // Leaf node
        }
        else
        {
            int mid = (start + end) / 2; // Calculate the mid point
            if (pos <= mid)
            {
                updateLCMAttack(2 * idx + 1, start, mid, pos, value); // Update left child
            }
            else
            {
                updateLCMAttack(2 * idx + 2, mid + 1, end, pos, value); // Update right child
            }
            lcmAttackTree[idx] = lcm(lcmAttackTree[2 * idx + 1], lcmAttackTree[2 * idx + 2]); // Combine results
        }
    }

public:
    SegmentTree(const vector<pair<int, int>> &values) : a(values) // Constructor to build the segment trees
    {
        int n = values.size(); // Get the size of the array
        // Resize trees for different operations
        sumAttackTree.resize(4 * n);  // Resize the sum attack tree
        sumHealthTree.resize(4 * n);  // Resize the sum health tree
        maxAttackTreeIndex.resize(4 * n);  // Resize the max attack tree
        maxHealthTreeIndex.resize(4 * n);  // Resize the max health tree
        minAttackTreeIndex.resize(4 * n);  // Resize the min attack tree
        minHealthTreeIndex.resize(4 * n);  // Resize the min health tree
        gcdAttackTree.resize(4 * n);  // Resize the GCD attack tree
        gcdHealthTree.resize(4 * n);  // Resize the GCD health tree
        lcmAttackTree.resize(4 * n);  // Resize the LCM attack tree
        lcmHealthTree.resize(4 * n);  // Resize the LCM health tree

        // Build the segment trees
        buildSumAttack(1, 0, n - 1);  // Build the sum attack tree
        buildSumHealth(1, 0, n - 1);  // Build the sum health tree
        buildMaxAttack(1, 0, n - 1);  // Build the max attack tree 
        buildMaxHealth(1, 0, n - 1);  // Build the max health tree
        buildMinAttack(1, 0, n - 1);  // Build the min attack tree
        buildMinHealth(1, 0, n - 1);  // Build the min health tree
        buildGcdAttack(1, 0, n - 1); // Assuming you have this
        buildGcdHealth(1, 0, n - 1); // Assuming you have this
        buildLcmAttack(1, 0, n - 1); // Assuming you have this
        buildLcmHealth(1, 0, n - 1); // Assuming you have this
    }

    // Query functions
    long long querySumAttack(int l, int r) // Function to query the sum of attacks in a given range
    {
        return querySumAttack(1, 0, a.size() - 1, l, r); // Call the helper function
    }

    long long querySumHealth(int l, int r)  // Function to query the sum of healths in a given range
    {
        return querySumHealth(1, 0, a.size() - 1, l, r);  // Call the helper function
    }

    int queryMaxAttackIndex(int l, int r)  // Function to query the index of max attack in a given range
    {
        return queryMaxAttackIndex(1, 0, a.size() - 1, l, r);  // Call the helper function
    }

    int queryMaxHealthIndex(int l, int r)  // Function to query the index of max health in a given range
    {
        return queryMaxHealthIndex(1, 0, a.size() - 1, l, r);  // Call the helper function
    }

    int queryMinAttackIndex(int l, int r)  // Function to query the index of min attack in a given range
    {
        return queryMinAttackIndex(1, 0, a.size() - 1, l, r);  // Call the helper function
    }

    int queryMinHealthIndex(int l, int r)  // Function to query the index of min health in a given range
    {
        return queryMinHealthIndex(1, 0, a.size() - 1, l, r); // Call the helper function
    }

    int queryGcdAttack(int l, int r) // Function to query the GCD of attacks in a given range
    {
        return queryGcdAttack(1, 0, a.size() - 1, l, r); // Call the helper function
    }

    int queryGcdHealth(int l, int r) // Function to query the GCD of healths in a given range
    {
        return queryGcdHealth(1, 0, a.size() - 1, l, r); // Call the helper function
    }

    int queryLcmAttack(int l, int r) // Function to query the LCM of attacks in a given range
    {
        return queryLcmAttack(1, 0, a.size() - 1, l, r); // Call the helper function
    }

    int queryLcmHealth(int l, int r) // Function to query the LCM of healths in a given range
    {
        return queryLcmHealth(1, 0, a.size() - 1, l, r); // Call the helper function
    }

    // Update function
    void update(int pos, int attack, int health) // Function to update the values at a specific position
    {
        a[pos] = {attack, health};                        // Update the values in the array
        updateSumAttack(1, 0, a.size() - 1, pos, attack); // Ensure the correct tree is updated
        updateMaxAttackIndex(pos, attack, a, 0, a.size() - 1, 0); // Update the maximum trees
        updateMaxHealthIndex(pos, health, a, 0, a.size() - 1, 0); // Update the maximum trees

        // Update the minimum trees
        updateMinAttackIndex(pos, attack, a, 0, a.size() - 1, 0); // Update the minimum trees
        updateMinHealthIndex(pos, health, a, 0, a.size() - 1, 0); // Update the minimum trees
        updateGCDHealth(1, 0, a.size() - 1, pos, health); // Update the GCD health tree
        updateLCMHealth(1, 0, a.size() - 1, pos, health); // Update the LCM health tree
        updateGCDAttack(1, 0, a.size() - 1, pos, attack); // Update the GCD attack tree
        updateLCMAttack(1, 0, a.size() - 1, pos, attack); // Update the LCM attack tree
    }
};
int main() // Main function to test the segment tree
{
    const int numSoldiers = 100000; // Number of soldiers in each team
    vector<pair<int, int>> team1(numSoldiers); // Vector to store the soldiers of team 1
    vector<pair<int, int>> team2(numSoldiers); // Vector to store the soldiers of team 2

    ifstream infile1("team1.txt"); // Read from team1.txt
    if (!infile1.is_open()) // Check if the file is open
    {
        cerr << "Error opening team1.txt!" << endl; // Print an error message
        return 1; // Return an error code
    }

    ifstream infile2("team2.txt"); // Read from team2.txt
    if (!infile2.is_open()) // Check if the file is open
    {
        cerr << "Error opening team2.txt!" << endl; // Print an error message
        return 1; // Return an error code
    }

    // Read from team1.txt for Team 1
    for (int i = 0; i < numSoldiers; i++) // Loop through the soldiers
    {
        int attack, health; // Variables to store the attack and health
        infile1 >> attack >> health; // Read the values from the file
        team1[i] = make_pair(attack, health); // Store the values in the vector
    }
    infile1.close(); // Close the file

    // Read from team2.txt for Team 2
    for (int i = 0; i < numSoldiers; i++) // Loop through the soldiers
    {
        int attack, health; // Variables to store the attack and health
        infile2 >> attack >> health; // Read the values from the file
        team2[i] = make_pair(attack, health); // Store the values in the vector
    }
    infile2.close(); // Close the file

    SegmentTree segtree1(team1); // Create a segment tree for team 1
    SegmentTree segtree2(team2); // Create a segment tree for team 2
    int n1 = 100;  // Number of rounds for team 1
    int teamAscore = 0; // Score for team 1
    int idx1 = 0; // Index for team 1
    cout << "There are 100000 soldiers on each team" << endl; // Print the number of soldiers
    cout << "Every time we will send a range of indexes of size 100 to attack and there will be 100 soldiers to defend on the other side" << endl; // Print the rules
    cout << "Score points are given based on sum of attack and sum of health of both teams" << endl; // Print the rules
    cout << "First 100 rounds are held for team A and 100 rounds are held for team B" << endl; // Print the rules
    cout << "After every attack, we will increase the attack of soldiers to 100 linearly for both teams" << endl; // Print the rules
    cout << "We will have LCM round and GCD round as surprise rounds based on LCM and GCD of two teams we will give them points" << endl; // Print the rules

    while (n1--) // Loop through the rounds for team 1
    {
        int l1, r1; // Variables to store the range of indexes
        cin >> l1 >> r1; // Read the range of indexes
        cout << segtree1.querySumAttack(l1, r1) << endl; // Query the sum of attacks
        int a1 = segtree1.querySumAttack(l1, r1); // Store the sum of attacks
        segtree1.update(idx1, 100, 0); // Update attack of team1 at idx1

        int l2, r2; // Variables to store the range of indexes
        cin >> l2 >> r2; // Read the range of indexes
        cout << segtree2.querySumHealth(l1, r1) << endl; // Query the sum of healths
        int a2 = segtree2.querySumHealth(l1, r1); // Store the sum of healths
        teamAscore += (a1 - a2); // Update the score for team 1
        idx1 += 100; // Increase the index by 100

        if (n1 % 10 == 0) // Surprise round
        {
            int l1, r1, l2, r2; // Variables to store the range of indexes
            cin >> l1 >> r1 >> l2 >> r2; // Read the range of indexes
            int a1 = segtree1.queryLcmAttack(l1, r1); // Query the LCM of attacks
            int a2 = segtree2.queryLcmHealth(l2, r2); // Query the LCM of healths
            if (a1 > a2) // Compare the LCM values
            {
                teamAscore += 50; // Update the score for team 1
            }
        }
        cout << "teamAscore after round: " << teamAscore << endl; // Print the score for team 1
    }
 
    int n2 = 100; // Number of rounds for team 2
    int teamBscore = 0;     // Score for team 2
    while (n2--) // Loop through the rounds for team 2
    {
        int l1, r1; // Variables to store the range of indexes
        cin >> l1 >> r1;    // Read the range of indexes
        cout << segtree2.querySumAttack(l1, r1) << endl; // Query the sum of attacks
        int a1 = segtree2.querySumAttack(l1, r1); // Store the sum of attacks
        segtree2.update(n2, 0, 0); // Assuming updates are needed for team2

        int l2, r2; // Variables to store the range of indexes
        cin >> l2 >> r2; // Read the range of indexes
        cout << segtree1.querySumHealth(l1, r1) << endl; // Query the sum of healths
        int a2 = segtree1.querySumHealth(l1, r1); // Store the sum of healths
        teamBscore += (a1 - a2); // Update the score for team 2

        if (n2 % 11 == 0) // Surprise round
        {
            int l1, r1, l2, r2; // Variables to store the range of indexes
            cin >> l1 >> r1 >> l2 >> r2; // Read the range of indexes
            int a1 = segtree1.queryGcdAttack(l1, r1); // Query the GCD of attacks
            int a2 = segtree2.queryGcdHealth(l2, r2); // Query the GCD of healths
            if (a2 > a1) // Compare the GCD values
            {
                teamBscore += 50; // Update the score for team 2
            }
        }
        cout << "teamBscore after round: " << teamBscore << endl ; // Print the score for team 2
    }

    if (teamAscore > teamBscore) // Compare the scores
    {
        cout << "team A won the game" << endl; // Print the winner
    }
    else if (teamAscore == teamBscore) // Compare the scores
    {
        cout << "match tied" << endl; // Print the result
    }
    else // Compare the scores
    {
        cout << "team B won the game" << endl; // Print the winner
    }

    return 0; // Return 0 to indicate success
}