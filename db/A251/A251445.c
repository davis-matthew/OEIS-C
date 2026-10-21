/*
 * A251445  Number of (n+1)X(2+1) 0..2 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements less than the absolute difference
 *          of its antidiagonal elements.
 * https://oeis.org/A251445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic‑programming over 27 states.
   a(n) is the number of (n+1)×3 arrays over {0,1,2} with the
   described 2×2 subblock constraint.
   The 27 possible rows of width 3 form a directed graph where
   i→j is allowed iff rows i (top) and j (bottom) together satisfy
   the constraint.  a(n) equals the sum of all entries of the
   adjacency matrix raised to the power n, i.e. the number of
   length‑n walks in this graph.  The algorithm keeps a pair of
   size‑27 vectors of type seq_t and iteratively multiplies by the
   adjacency matrix, memoising results so that calls may come in any
   order. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* External integer types */
typedef idx_t  idx_t;   /* defined by the build, typically uint64_t */
typedef seq_t seq_t;    /* defined by the build, unsigned of various sizes */

/* Memoised computation of a(n) */
seq_t a(idx_t n)
{
    /* The sequence is indexed from 1. For safety return 0 for n<1. */
    if (n == 0) return 0;

    /* Static state to remember across calls. */
    static bool init = true;
    static bool allowed[27][27];
    static seq_t dp_cur[27];
    static seq_t dp_next[27];
    static seq_t *cache = NULL;     /* cache[0] unused, cache[1] = a(1), … */
    static size_t cache_cap = 0;    /* allocated size of cache */
    static idx_t cache_max = 0;     /* highest n already computed */

    /* ------------------------------------------------------------------
       First time through: pre‑compute the adjacency matrix and
       initialise the DP vectors.  The initial vector represents the
       possibility of *any* first row, so each entry is 1.
     ------------------------------------------------------------------ */
    if (init) {
        /* Build adjacency table. */
        for (int i = 0; i < 27; ++i) {
            int a0 = i / 9;
            int a1 = (i / 3) % 3;
            int a2 = i % 3;
            for (int j = 0; j < 27; ++j) {
                int b0 = j / 9;
                int b1 = (j / 3) % 3;
                int b2 = j % 3;
                bool ok = true;
                /* Two overlapping 2×2 blocks: columns 0–1 and 1–2. */
                for (int col = 0; col < 2 && ok; ++col) {
                    int tl  = (col == 0) ? a0 : a1;  /* top‑left   */
                    int tr  = (col == 0) ? a1 : a2;  /* top‑right  */
                    int bl  = (col == 0) ? b0 : b1;  /* bottom‑left*/
                    int br  = (col == 0) ? b1 : b2;  /* bottom‑right*/
                    int min_diag = (tl < br) ? tl : br;
                    int diff = (tr > bl) ? tr - bl : bl - tr;
                    if (min_diag < diff) ok = false;
                }
                allowed[i][j] = ok;
            }
        }

        /* Initial vector: any first row is possible. */
        for (int i = 0; i < 27; ++i) dp_cur[i] = 1;

        /* Allocate cache for at least the first term. */
        cache_cap = 2;                         /* indices 0 and 1 */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]  = 0;                         /* unused */
        cache[1]  = 0;                         /* will be filled when needed */
        cache_max = 0;                         /* none computed yet */

        init = false;
    }

    /* ------------------------------------------------------------------
       Grow the cache if the caller requests a larger n than we can
       currently hold.  We use realloc; it always succeeds because the
       called n is within the problem limits (< 2⁶⁴ and ≤ 10000 in
       practice).  The extra space (n+1) is safe for casting to size_t.
     ------------------------------------------------------------------ */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        cache_cap = required + 1;   /* a bit of slack for future calls */
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* ------------------------------------------------------------------
       Incrementally compute values up to the requested n.
       Each iteration corresponds to adding a new row to the array.
     ------------------------------------------------------------------ */
    while (cache_max < n) {
        /* dp_next = dp_cur * allowed */
        for (int j = 0; j < 27; ++j) dp_next[j] = 0;
        for (int i = 0; i < 27; ++i) {
            for (int j = 0; j < 27; ++j)
                if (allowed[i][j])
                    dp_next[j] += dp_cur[i];
        }

        /* Sum all entries: this is a(cache_max+1). */
        seq_t sum = 0;
        for (int j = 0; j < 27; ++j) sum += dp_next[j];
        cache[cache_max + 1] = sum;

        /* Prepare for next step. */
        for (int j = 0; j < 27; ++j) dp_cur[j] = dp_next[j];
        ++cache_max;
    }

    /* Return the requested term. */
    return cache[n];
}
