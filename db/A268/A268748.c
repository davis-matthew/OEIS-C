/*
 * A268748  Number of nX6 binary arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling two no more
 *          than once.
 * https://oeis.org/A268748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×6 binary arrays with at most one adjacency pair of
 *            1s
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation is performed by a transfer‑matrix DP over row masks.  */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/*  The build supplies typedefs for seq_t and idx_t  */
extern void seq_zero(seq_t *x);               /* helper to set a seq_t to zero */
extern seq_t seq_inc(seq_t *x);              /* helper to increment a seq_t by one */

/* Maximum n that will be queried (by specification)  */
#define MAX_N 10000

/*  Static state for memoisation  */
static seq_t a_cache[MAX_N + 1];          /* cache of a(1) .. a(n)  */
static idx_t cached_n = 0;                /* highest n cached  */

static seq_t dp0[64], dp1[64];            /* dp after the current number of rows, adjacency count 0 or 1  */
static seq_t next0[64], next1[64];        /* temporary arrays for the next row  */
static int horiz_mask[64];                /* horizontal adjacency count of a mask  */
static int pair_pop[64][64];              /* popcount(prev & cur)  */

/*  Initialize lookup tables and cache  */
static void init_lookup(void)
{
    /* pre‑compute horizontal adjacencies for every mask  */
    for (int mask = 0; mask < 64; ++mask) {
        int h = 0;
        for (int i = 0; i < 5; ++i)
            if ( ((mask >> i) & 1) && ((mask >> (i + 1)) & 1) )
                h++;
        horiz_mask[mask] = h;
    }

    /* pre‑compute popcounts of overlaps */
    for (int a = 0; a < 64; ++a)
        for (int b = 0; b < 64; ++b)
            pair_pop[a][b] = __builtin_popcount((unsigned)(a & b));

    /* initialise dp for the first row (n = 1)  */
    for (int i = 0; i < 64; ++i) {
        dp0[i] = 0;  /* seq_zero on builtin types is just 0 assignment */
        dp1[i] = 0;
    }
    for (int mask = 0; mask < 64; ++mask) {
        int h = horiz_mask[mask];
        if (h == 0) {
            dp0[mask] = 1;
        } else if (h == 1) {
            dp1[mask] = 1;
        }
    }

    /* compute a(1)  */
    seq_t total = 0;
    for (int i = 0; i < 64; ++i)
        total += dp0[i] + dp1[i];
    a_cache[0] = 0;
    a_cache[1] = total;
    cached_n = 1;
}

/*  Extend precomputed terms up to required n  */
static void extend_to(idx_t n)
{
    if (!cached_n) init_lookup();   /* first time we need to initialise */

    for (idx_t row = cached_n; row < n; ++row) {
        /* zero the next arrays */
        for (int i = 0; i < 64; ++i) {
            next0[i] = 0;
            next1[i] = 0;
        }

        for (int prev = 0; prev < 64; ++prev) {
            /* If this previous mask contributes nothing, skip */
            if (dp0[prev] == 0 && dp1[prev] == 0) continue;

            for (int cur = 0; cur < 64; ++cur) {
                int h = horiz_mask[cur];
                if (h > 1) continue;                    /* cur alone already exceeds limit */

                int v = pair_pop[prev][cur];            /* vertical adjacency between prev and cur */

                /* Transition from dp0[prev] (total adjacency 0) */
                if (dp0[prev] != 0) {
                    int sum = h + v;
                    seq_t add = dp0[prev];
                    if (sum == 0) {
                        next0[cur] += add;
                    } else if (sum == 1) {
                        next1[cur] += add;
                    }
                }

                /* Transition from dp1[prev] (total adjacency 1) */
                if (dp1[prev] != 0) {
                    int sum = 1 + h + v;
                    if (sum == 1) {                     /* impossible otherwise */
                        next1[cur] += dp1[prev];
                    }
                }
            }
        }

        /* swap arrays: dp now becomes next */
        for (int i = 0; i < 64; ++i) {
            dp0[i] = next0[i];
            dp1[i] = next1[i];
        }

        /* compute a(row+1) */
        seq_t total = 0;
        for (int i = 0; i < 64; ++i)
            total += dp0[i] + dp1[i];
        a_cache[row + 1] = total;
    }
    cached_n = n;
}

/*  Public function required by the problem statement  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n=0, return 0 safely */
    if (n <= cached_n)
        return a_cache[n];
    extend_to(n);
    return a_cache[n];
}
