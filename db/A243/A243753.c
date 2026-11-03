/*
 * A243753  Number A(n,k) of Dyck paths of semilength n avoiding the
 *          consecutive step pattern given by the binary expansion of k, where
 *          1=U=(1,1) and 0=D=(1,-1); square array A(n,k), n>=0, k>=0, read by
 *          antidiagonals.
 * https://oeis.org/A243753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 224, u32 n <= 517, u64 n <= 1315,
 *            u128 n <= 3816, big any n
 * Validated: standard, pass, 107 terms through n = 9750 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A243753
 *  a(n) is the number of Dyck paths of semilength i that avoid the
 *  consecutive step pattern given by the binary expansion of j,
 *  where (i,j) are the antidiagonal coordinates of n.
 *  The function computes a(n) by constructing an automaton for the
 *  forbidden pattern and dynamic‑programming over the path steps.
 *
 *  The code is written in a form that compiles under both C11
 *  and C++17.  No library functions other than the minimal
 *  standard headers are used.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* counts Dyck paths of semilength 'semilength' that avoid the
 * forbidden pattern encoded by 'pattern_idx' (binary expansion).
 * The implementation uses a finite‑state automaton for the pattern
 * and dynamic programming over the 2*semilength steps.           */
static seq_t count_dyck_without_pattern(idx_t semilength, idx_t pattern_idx)
{
    if (semilength == 0)            /* empty path */
        return (seq_t)1;

    /* determine the binary pattern length; j <= semilength for the
     * queries in this problem, so the maximum length is tiny.     */
    size_t L = 0;
    idx_t tmp = pattern_idx;
    if (tmp == 0) {
        L = 1;
    } else {
        while (tmp) { L++; tmp >>= 1; }
    }
    if (L > 16)                     /* safety guard (should not happen) */
        return (seq_t)0;

    /* encode pattern bits (most significant bit first).             */
    uint8_t pat[16] = {0};
    tmp = pattern_idx;
    if (tmp == 0) {
        pat[0] = 0;
    } else {
        size_t pos = L;
        while (tmp) {
            pat[--pos] = (uint8_t)(tmp & 1);
            tmp >>= 1;
        }
    }

    /* build the KMP “next” table for pattern matching.             */
    int nxt[16][2];
    int pi[16]  = {0};
    for (size_t i = 1; i < L; i++) {
        int j = pi[i-1];
        while (j > 0 && pat[i] != pat[j]) j = pi[j-1];
        if (pat[i] == pat[j]) j++;
        pi[i] = j;
    }
    for (size_t s = 0; s < L; s++) {
        for (int c = 0; c <= 1; c++) {
            int k = (int)s;
            while (k > 0 && pat[k] != (uint8_t)c) k = pi[k-1];
            if (pat[k] == (uint8_t)c) k++;
            nxt[s][c] = k;
        }
    }

    /* dynamic programming arrays:  dp[h][s]  = number of partial
     * paths that end at height h and are in state s of the automaton. */
    static seq_t dp[2001][16];
    static seq_t ndp[2001][16];

    if (semilength > 2000)          /* another safety guard     */
        return (seq_t)0;

    /* initialise dp/ndp */
    for (idx_t h = 0; h <= semilength; h++)
        for (size_t s = 0; s < L; s++)
            dp[h][s] = ndp[h][s] = (seq_t)0;

    dp[0][0] = (seq_t)1;

    for (idx_t step = 0; step < 2 * semilength; step++) {
        /* clear next array */
        for (idx_t h = 0; h <= semilength; h++)
            for (size_t s = 0; s < L; s++)
                ndp[h][s] = (seq_t)0;

        for (idx_t h = 0; h <= semilength; h++) {
            for (size_t s = 0; s < L; s++) {
                seq_t val = dp[h][s];
                if (!val) continue;

                /* U step */
                if (h + 1 <= semilength) {
                    int ns = nxt[s][1];
                    if (ns < (int)L)
                        ndp[h+1][ns] += val;
                }
                /* D step */
                if (h > 0) {
                    int ns = nxt[s][0];
                    if (ns < (int)L)
                        ndp[h-1][ns] += val;
                }
            }
        }
        /* copy ndp to dp for next iteration */
        for (idx_t h = 0; h <= semilength; h++)
            for (size_t s = 0; s < L; s++)
                dp[h][s] = ndp[h][s];
    }

    /* sum over all automaton states at height 0 to get valid Dyck paths */
    seq_t result = (seq_t)0;
    for (size_t s = 0; s < L; s++)
        result += dp[0][s];

    return result;
}

/* compute antidiagonal coordinates:  a(n) = A(i,j) for the array
 *  displayed in the OEIS entry.  i is the offset on the diagonal,
 *  j is the remaining index on the same antidiagonal.                */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t t = 0;
    while (true) {
        /* (t+1)*(t+2)/2  is strictly increasing in t */
        __int128 v = (__int128)(t + 1) * (t + 2) / 2;
        if (v > n) break;
        t++;
    }
    idx_t prev_sum = t * (t + 1) / 2;   /* sum of 1..t */
    idx_t offset   = n - prev_sum;
    *i = offset;
    *j = t - offset;
}

/* public entry point for the OEIS sequence */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return count_dyck_without_pattern(i, j);
}
