/*
 * A269041  Number of 7Xn 0..2 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A269041: number of 7×n 0..2 arrays with exactly one
 *  horizontally, diagonally or antidiagonally adjacent pair summing to 2.
 *
 *  Computation is performed by a dynamic‑programming automaton.
 *  Each column is represented by a base‑3 pattern of length 7
 *  (2187 possibilities).  For every pair of consecutive columns the
 *  number of “special” adjacencies (sum = 2) is at most 1 or 0 in the
 *  transitions we keep.  The automaton tracks whether a special
 *  adjacency has already been used; in the end the sum of all states
 *  with exactly one such adjacency gives the desired count.
 *
 *  The algorithm runs in O(n · T) where T is the number of
 *  pattern‐to‑pattern transitions producing 0 or 1 specials
 *  (≈ 1.5 × 10⁴), well below the 2 s limit for n ≈ 10 000.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---- constants ----------------------------------------------------------- */
enum {
    ROWS      = 7,
    PATTERNS  = 3 * 3 * 3 * 3 * 3 * 3 * 3               /* 3^7 = 2187 */
};

/* ---- pattern representation ---------------------------------------------- */
static int pattern_digits[PATTERNS][ROWS];   /* base‑3 digits of each pattern */

/* ---- adjacency statistics ----------------------------------------------- */
static int zero_counts[PATTERNS];            /* how many next patterns give 0 specials */
static int one_counts[PATTERNS];             /* how many next patterns give 1 special */
static int *zero_neigh[PATTERNS];             /* lists of next patterns with 0 specials */
static int *one_neigh[PATTERNS];              /* lists of next patterns with 1 special */

/* ---- DP tables ------------------------------------------------------------ */
static seq_t dpPrevArr[PATTERNS][2];
static seq_t dpCurArr[PATTERNS][2];
static seq_t (*dpPrev)[2] = dpPrevArr;
static seq_t (*dpCur)[2]  = dpCurArr;

/* ---- cache of computed sequence values ----------------------------------- */
static seq_t *seqCache   = NULL;
static size_t cacheCap   = 0;
static idx_t  maxComputed = 0;   /* number of computed columns (1‑based) */

/* ---- helper: number of special adjacencies between two patterns --------- */
static inline int edge_count(int prev, int cur)
{
    const int *a = pattern_digits[prev];
    const int *b = pattern_digits[cur];
    int c = 0;

    /* horizontal */
    for (int i = 0; i < ROWS; ++i)
        if (a[i] + b[i] == 2) ++c;

    /* diagonal down (prev[i] with cur[i+1]) */
    for (int i = 0; i < ROWS - 1; ++i)
        if (a[i] + b[i + 1] == 2) ++c;

    /* diagonal up (prev[i] with cur[i-1]) */
    for (int i = 1; i < ROWS; ++i)
        if (a[i] + b[i - 1] == 2) ++c;

    return c;
}

/* ---- cache utilities ----------------------------------------------------- */
static void cache_set(idx_t idx, seq_t value)
{
    if ((size_t)(idx + 1) > cacheCap) {
        size_t newCap = (cacheCap == 0) ? 32 : cacheCap << 1;
        while (newCap <= (size_t)idx) newCap <<= 1;
#ifdef __cplusplus
        seq_t *newCache = new seq_t[newCap];
        if (seqCache) {
            for (size_t i = 0; i < cacheCap; ++i) newCache[i] = seqCache[i];
            delete[] seqCache;
        }
#else
        seq_t *newCache = (seq_t *)realloc(seqCache, newCap * sizeof(seq_t));
#endif
        seqCache = newCache;
        cacheCap = newCap;
    }
    seqCache[idx] = value;
}

/* ---- initialisation ------------------------------------------------------ */
static void init(void)
{
    /* build all 2187 patterns */
    for (int p = 0; p < PATTERNS; ++p) {
        int v = p;
        for (int r = 0; r < ROWS; ++r) {
            pattern_digits[p][r] = v % 3;
            v /= 3;
        }
    }

    /* first pass: count zero and one neighbours */
    for (int prev = 0; prev < PATTERNS; ++prev) {
        zero_counts[prev] = 0;
        one_counts[prev]  = 0;
    }

    for (int prev = 0; prev < PATTERNS; ++prev) {
        for (int cur = 0; cur < PATTERNS; ++cur) {
            int c = edge_count(prev, cur);
            if (c == 0)   ++zero_counts[prev];
            else if (c == 1) ++one_counts[prev];
        }
    }

    /* allocate neighbour tables */
    for (int i = 0; i < PATTERNS; ++i) {
        zero_neigh[i] = (zero_counts[i] > 0) ? (int *)malloc(zero_counts[i] * sizeof(int)) : NULL;
        one_neigh[i]  = (one_counts[i]  > 0) ? (int *)malloc(one_counts[i]  * sizeof(int)) : NULL;
    }

    /* second pass: fill neighbour lists */
    int *zc = (int *)calloc(PATTERNS, sizeof(int));
    int *oc = (int *)calloc(PATTERNS, sizeof(int));

    for (int prev = 0; prev < PATTERNS; ++prev) {
        for (int cur = 0; cur < PATTERNS; ++cur) {
            int c = edge_count(prev, cur);
            if (c == 0) {
                zero_neigh[prev][zc[prev]++] = cur;
            } else if (c == 1) {
                one_neigh[prev][oc[prev]++]  = cur;
            }
        }
    }

    free(zc);
    free(oc);

    /* initialise DP for the first column: one way per pattern, 0 specials used */
    for (int p = 0; p < PATTERNS; ++p) {
        dpPrev[p][0] = 1;
        dpPrev[p][1] = 0;
    }

    /* cache a(1) = 0 */
    cache_set(1, 0);
    maxComputed = 1;
}

/* ---- extend DP to desired column ---------------------------------------- */
static void extend_to(idx_t target)
{
    while (maxComputed < target) {
        int next = (int)(maxComputed + 1);

        /* reset current DP table */
        memset(dpCur, 0, sizeof(dpCurArr));

        for (int prev = 0; prev < PATTERNS; ++prev) {
            seq_t val0 = dpPrev[prev][0];
            seq_t val1 = dpPrev[prev][1];

            if (val0) {
                /* transitions that add 0 or 1 specials */
                for (int i = 0; i < zero_counts[prev]; ++i) {
                    int cur = zero_neigh[prev][i];
                    dpCur[cur][0] += val0;
                }
                for (int i = 0; i < one_counts[prev]; ++i) {
                    int cur = one_neigh[prev][i];
                    dpCur[cur][1] += val0;
                }
            }
            if (val1) {
                /* already used one special; can only go through 0‑special edges */
                for (int i = 0; i < zero_counts[prev]; ++i) {
                    int cur = zero_neigh[prev][i];
                    dpCur[cur][1] += val1;
                }
                /* transitions producing an extra special are omitted */
            }
        }

        /* swap tables */
        seq_t (*tmp)[2] = dpPrev;
        dpPrev = dpCur;
        dpCur  = tmp;

        /* compute the answer for this column */
        seq_t sum = 0;
        for (int p = 0; p < PATTERNS; ++p)
            sum += dpPrev[p][1];

        /* store in cache */
        cache_set(next, sum);
        maxComputed = next;
    }
}

/* ---- public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset starts at 1; undefined for 0 */

    if (!seqCache) init();

    if (n <= maxComputed)
        return seqCache[n];

    extend_to(n);
    return seqCache[n];
}
