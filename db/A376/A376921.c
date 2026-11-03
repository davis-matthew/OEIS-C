/*
 * A376921  Number T(n,k) of binary words of length n avoiding distance (i+1)
 *          between "1" digits if the i-th bit is set in the binary
 *          representation of k; triangle T(n,k), n>=0,
 *          0<=k<=ceiling(2^(n-1))-1, read by rows.
 * https://oeis.org/A376921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) counts binary words of length r avoiding forbidden
 *            distances * according to the key k derived from the index n,
 *            i.e. the entry T(r,k) of * the triangle described in A376921.
 *            The function uses a dynamic programming * method that is
 *            O(r·2^(r‑1)), where r ≈ log2(n). For any n ≥ 0 it returns * t...
 * Range:     u8 n <= 127, u16 n <= 32767, u32 n <= 65535, u64 n <= 65535,
 *            u128 n <= 65535, big any n
 * Validated: standard, pass, 80 terms through n = 9772 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Row r (r >= 1) holds T(r,k), 0 <= k < 2^(r-1), at n = 2^(r-1) + k. Each
 * word w of length r has the set M(w) of the distances d (bit d-1) between
 * two of its 1s, M(w) has bit d-1 iff w & (w >> d) != 0; and T(r,k) is the
 * number of w with M(w) & k = 0, i.e. with M(w) inside the complement of k.
 * So the counts f[m] of the words with M(w) = m, summed over subsets (a zeta
 * transform), give the whole row at once, in O(r 2^r); the last row is kept.
 * (The original redid a DP over 2^(r-1) states for every term, in fixed
 * arrays of 8192, which row 15 already outgrows.) */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *row = NULL;             /* row[k] = T(cur_r, k) */
static uint64_t cur_r = 0;               /* 0: none yet */

static void build_row(uint64_t r)
{
    size_t half = (size_t)1 << (r - 1);  /* masks of r - 1 distance bits */
    uint64_t *g = (uint64_t *)calloc(half, sizeof *g);
    if (!g) abort();
    for (uint64_t w = 0; w < ((uint64_t)1 << r); ++w) {
        size_t m = 0;
        for (uint64_t d = 1; d < r; ++d)
            if (w & (w >> d)) m |= (size_t)1 << (d - 1);
        ++g[m];
    }
    for (size_t b = 1; b < half; b <<= 1)        /* g[S] = sum of f[m], m in S */
        for (size_t S = 0; S < half; ++S)
            if (S & b) g[S] += g[S ^ b];
    uint64_t *t = (uint64_t *)malloc(half * sizeof *t);
    if (!t) abort();
    for (size_t k = 0; k < half; ++k) t[k] = g[(half - 1) & ~k];
    free(g);
    free(row);
    row = t;
    cur_r = r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;                      /* row 0: the empty word */
    uint64_t r = (uint64_t)(64 - __builtin_clzll((unsigned long long)n));
    if (r != cur_r) build_row(r);
    return (seq_t)row[(uint64_t)n - ((uint64_t)1 << (r - 1))];
}
