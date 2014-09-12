//
//  main.cpp
//  rankUnrank
//
//  Created by Joseph Canero on 9/11/14.
//  Copyright (c) 2014 Joseph Canero. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <string>
#include <set>

using namespace std;

// memoization of factorials
long factorial_array[100];

// counts the number of 1s in a number
int count_ones(int num) {
    int num_ones = 0;
    while (num) {
        if (num & 1) num_ones += 1;
        num >>= 1;
    }
    return num_ones;
}

// computes the factorial of a number. uses memoization to remove redundant calculations
long factorial(int num) {
    if (factorial_array[num] != 0) return factorial_array[num];
    long result;
    
    for (int i = 2; i <= num; i++) {
        result = i * factorial_array[i - 1];
        factorial_array[i] = result;
    }
    
    return factorial_array[num];
}

// ranking a permutation involves determining the position of the given permutation in the lexicographically sorted set of all permutations
long rank_permutation(string permutation, int n);

// unranking a permutation involves determining which permutation correspond to the given position in the sorted order
string unrank_permutation(long n);

long rank_permutation(string permutation, int n) {
    unsigned values_used = 0x00, set = 0x00, temp1;
    unsigned temp, test = 0;
    long rank = 0;
    int i = 0, c;
    int num_ones1, num_ones2;
    
    for(int j = 0; j < n; j++) {
        c = permutation[j] - '0';
        temp = 1;
        temp <<= n - c;
        values_used |= temp;
        set = ~values_used;
        set &= ((long)pow(2, n)) - 1;
        temp = values_used;
        temp >>= n - c;
        temp1 = set;
        temp1 >>= n - c;
        test = temp & temp1;
        num_ones1 = count_ones(test);
        num_ones2 = count_ones(temp1);
        rank += factorial((n - 1) - i) * (num_ones2 - num_ones1);
        
        i++;
    }
    
    return rank;
}

string unrank_permutation(long rank, int n) {
    set<char> perm_set;
    string result(n, '0');
    long rank_deficit = 0, temp_deficit = 0;
    string permutation = "";
    int counter = n - 1, to_remove = 0;
    
    for (int i = 1; i <= n; i++) perm_set.insert(char(i + '0'));
    
    
    while (counter >= 0) {
        for (int i = 0; (i <= n - 1) && temp_deficit <= rank; i++) {
            temp_deficit = i * factorial(counter);
            if (temp_deficit <= rank) {
                to_remove = i;
                rank_deficit = temp_deficit;
            }
        }
        rank -= rank_deficit;
        temp_deficit = 0;
        set<char>::iterator iter = perm_set.begin();

        while (iter != perm_set.end() && to_remove > 0) { iter++; to_remove -= 1; }
        result[(n - 1) - counter] = *iter;
        perm_set.erase(iter);
        counter -= 1;
    }
    return result;
}

int main(void) {
    factorial_array[0] = factorial_array[1] = 1;
    for (int i = 2; i < 100; i++) factorial_array[i] = 0;
    
    cout << "RANK OF 625143: " << rank_permutation("625143", 6) << endl;
    cout << "UNRANKING OF 645: " << unrank_permutation(643, 6) << endl;
    
    // ranking and unranking are inverse operations
    
    cout << "RANK OF UNRANKING OF 643: " << rank_permutation(unrank_permutation(643, 6), 6) << endl;
    cout << "UNRANK OF RANKING OF 625143: " << unrank_permutation(rank_permutation("625143", 6), 6) << endl;
}
