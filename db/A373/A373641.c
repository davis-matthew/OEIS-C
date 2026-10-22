/*
 * A373641  The number of positive n-digit integers whose digit product is n.
 * https://oeis.org/A373641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 95, u64 n <= 767,
 *            u128 n <= 8191, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373641: Number of positive n‑digit integers whose digit product is n */

/* Authors: This implementation uses only integer arithmetic and
 * dynamic programming over the prime factor exponents of n.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Exponent vectors for digits 1..9: [e2, e3, e5, e7] */
static const int digit_vec[10][4] = {
    {0,0,0,0},      /* 0 unused */
    {0,0,0,0},      /* 1  */
    {1,0,0,0},      /* 2  */
    {0,1,0,0},      /* 3  */
    {2,0,0,0},      /* 4  */
    {0,0,1,0},      /* 5  */
    {1,1,0,0},      /* 6  */
    {0,0,0,1},      /* 7  */
    {3,0,0,0},      /* 8  */
    {0,2,0,0}       /* 9  */
};

/* Memoisation tables for the recursive DP.
 * Limit sizes are enough for all n <= 10000 (e2<=14, e3<=8, e5<=5, e7<=4, k<=14). */
static seq_t memo[16][10][6][6][16];
static bool memo_used[16][10][6][6][16];

/* Recursive DP: number of ordered sequences of length k using digits 2..9
 * whose prime exponents sum to (e2,e3,e5,e7). */
static seq_t count_seq(int e2, int e3, int e5, int e7, int k)
{
    if (k == 0)
        return (e2 == 0 && e3 == 0 && e5 == 0 && e7 == 0) ? (seq_t)1 : (seq_t)0;

    if (e2 < 0 || e3 < 0 || e5 < 0 || e7 < 0)
        return (seq_t)0;

    if (memo_used[e2][e3][e5][e7][k])
        return memo[e2][e3][e5][e7][k];

    seq_t res = (seq_t)0;

    for (int d = 2; d <= 9; ++d) {
        int ne2 = e2 - digit_vec[d][0];
        int ne3 = e3 - digit_vec[d][1];
        int ne5 = e5 - digit_vec[d][2];
        int ne7 = e7 - digit_vec[d][3];
        if (ne2 < 0 || ne3 < 0 || ne5 < 0 || ne7 < 0)
            continue;
        res += count_seq(ne2, ne3, ne5, ne7, k - 1);
    }

    memo_used[e2][e3][e5][e7][k] = true;
    memo[e2][e3][e5][e7][k] = res;
    return res;
}

/* Compute binomial coefficient C(n, k) using exact division with gcd at each step. */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;
        res *= num;
        res /= den;
    }
    return res;
}

/* ----------------------------------------------------------- */
/* Public function required by the kernel */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;  /* only all-ones number */

    /* Factorisation of n, keeping only primes 2,3,5,7 */
    idx_t tmp = n;
    int e2 = 0, e3 = 0, e5 = 0, e7 = 0;
    while (tmp % 2 == 0) { ++e2; tmp /= 2; }
    while (tmp % 3 == 0) { ++e3; tmp /= 3; }
    while (tmp % 5 == 0) { ++e5; tmp /= 5; }
    while (tmp % 7 == 0) { ++e7; tmp /= 7; }
    if (tmp != 1)   /* contains a prime > 7 */
        return (seq_t)0;

    int sumExp = e2 + e3 + e5 + e7;
    int kmax = sumExp;          /* at most one prime factor per non‑1 digit */
    if (kmax > n) kmax = n;     /* for safety, though n >= kmax automatically */

    /* clear memoisation tables for this call */
    memset(memo_used, 0, sizeof(memo_used));

    seq_t ans = (seq_t)0;

    for (int k = 1; k <= kmax; ++k) {
        seq_t ways = count_seq(e2, e3, e5, e7, k);
        if (ways == 0)
            continue;
        seq_t comb = binom_idx(n, k);
        ans += comb * ways;
    }

    return ans;
}
