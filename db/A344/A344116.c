/*
 * A344116  Triangle read by rows: T(n,k) is the number of relations from an
 *          n-element set to a k-element set that are not onto functions.
 * https://oeis.org/A344116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = 2^(r*k) - k!·Stirling2(r,k) and
 *            (r,k) are the row and column reached by the n‑th triangle entry
 *            (rows are numbered starting at 1, columns at 1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 20, u64 n <= 43, u128 n <= 76,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

static seq_t **S = NULL;   /* Stirling numbers, S[i][j] = S(i,j) */
static seq_t *fact = NULL; /* factorials, fact[j] = j! */
static size_t curR = 0;    /* largest i for which S[i] is filled */

/* ensure that rows 0..level are available in S and fact */
static void ensure_precompute(size_t level) {
    if (level <= curR) return;

    /* extend S array of row pointers */
    S = (seq_t **)realloc(S, sizeof(seq_t *) * (level + 1));
    if (!S) exit(1);

    /* extend fact array */
    fact = (seq_t *)realloc(fact, sizeof(seq_t) * (level + 1));
    if (!fact) exit(1);

    /* base row 0 */
    if (curR == 0) {
        S[0] = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!S[0]) exit(1);
        S[0][0] = (seq_t)1;          /* S(0,0) = 1 */
        fact[0] = (seq_t)1;          /* 0! = 1 */
        curR = 0;
    }

    /* compute rows curR+1 .. level */
    for (size_t i = curR + 1; i <= level; ++i) {
        S[i] = (seq_t *)malloc(sizeof(seq_t) * (i + 1));
        if (!S[i]) exit(1);
        S[i][0] = (seq_t)0;
        for (size_t j = 1; j <= i; ++j) {
            seq_t part1 = (j <= i - 1) ? (seq_t)j * S[i - 1][j] : (seq_t)0;
            seq_t part2 = S[i - 1][j - 1];
            S[i][j] = part1 + part2;
        }
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    curR = level;
}

/* find the smallest r with r(r+1)/2 >= n (1‑based n) */
static size_t find_row(idx_t n) {
    size_t lo = 1, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n<1, but return 0 */

    size_t r = find_row(n);                       /* row index (1‑based) */
    unsigned __int128 prev_tri = (unsigned __int128)r * (r - 1) / 2;
    size_t k = (size_t)(n - (idx_t)prev_tri);     /* column index (1‑based) */

    ensure_precompute(r);                /* make S[r][k] and fact[k] ready */

    idx_t exp = (idx_t)(r * k);
    seq_t total_rel = pow2(exp);          /* 2^(r*k) */
    seq_t onto = fact[k] * S[r][k];       /* k! * S(r,k) */
    return total_rel - onto;
}
