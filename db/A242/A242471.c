/*
 * A242471  Number of length n+2 0..8 arrays with no three equal elements in a
 *          row and new values 0..8 introduced in 0..8 order.
 * https://oeis.org/A242471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 44,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Dynamic programming over length with state (prevprev, prev, max), 
   computing a(n) for any n by iterating up to the requested length. 
   The state tracks the two most recent symbols (0..8, plus a dummy for 
   the first position) and the maximum symbol seen so far (0..8).  */

/* Standard headers */
#include <stddef.h>
#define _GNU_SOURCE   /* for NULL definition if needed */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Constants for DP state */
#define DIM 10          /* indices 0..8 plus dummy 9 */
#define MAXSYM 9        /* symbols 0..8 */
#define DUMMY 9

/* Static memoisation structures */
static seq_t *cache = NULL;        /* a(1..cache_computed) stored, 1‑based */
static idx_t cache_cap = 0;        /* allocated size of cache (>= n+1) */
static idx_t cache_computed = 0;   /* largest n for which cache[n] is set */

/* DP arrays for current and next length */
static seq_t cur[DIM][DIM][MAXSYM];
static seq_t nxt[DIM][DIM][MAXSYM];
static idx_t cur_len = 0;          /* current length represented by cur */
static idx_t cur_n_max = 0;        /* largest n for which cache[n] is set */

/* Initialise the DP arrays and cache */
static void init_dp(void)
{
    /* clear DP arrays */
    for (int i = 0; i < DIM; ++i)
        for (int j = 0; j < DIM; ++j)
            for (int k = 0; k < MAXSYM; ++k) {
                cur[i][j][k] = 0;
                nxt[i][j][k] = 0;
            }
    /* first symbol must be 0 */
    cur[DUMMY][0][0] = 1;
    cur_len = 1;
    cur_n_max = 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for n < 1 */

    /* One‑time initialisation */
    if (cache_cap == 0) {
        cache_cap = 2;                          /* at least space for a[1] */
        cache = seq_alloc_new_array((size_t)cache_cap);
        if (!cache) return (seq_t)0;             /* allocation failure */
        init_dp();
    }

    /* Return cached value if already computed */
    if (n <= cache_computed)
        return cache[(size_t)n];

    /* Grow cache array if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        if (!new_cache) return (seq_t)0;
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        seq_free_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Extend DP up to the required length */
    idx_t target_len = n + 2;           /* length of strings for this n */
    for (idx_t len = cur_len + 1; len <= target_len; ++len) {

        /* zero nxt array */
        for (int i = 0; i < DIM; ++i)
            for (int j = 0; j < DIM; ++j)
                for (int k = 0; k < MAXSYM; ++k)
                    nxt[i][j][k] = 0;

        /* transition from cur to nxt */
        for (int a = 0; a < DIM; ++a) {
            for (int b = 0; b < DIM; ++b) {
                for (int m = 0; m < MAXSYM; ++m) {
                    seq_t cnt = cur[a][b][m];
                    if (cnt == 0) continue;
                    for (int z = 0; z < MAXSYM; ++z) {
                        /* rule: no triple equal */
                        if (a != DUMMY && a == b && b == z) continue;
                        /* rule: new symbols in order */
                        if (z > m && z != m + 1) continue;
                        int na = b;
                        int nb = z;
                        int nm = (z > m) ? z : m;
                        nxt[na][nb][nm] += cnt;
                    }
                }
            }
        }

        /* if length >= 3 (i.e., n >= 1), compute total and store */
        if (len >= 3) {
            seq_t total = 0;
            for (int i = 0; i < DIM; ++i)
                for (int j = 0; j < DIM; ++j)
                    for (int k = 0; k < MAXSYM; ++k)
                        total += nxt[i][j][k];
            idx_t cur_n = len - 2;            /* n = len - 2 */
            cache[(size_t)cur_n] = total;
            cache_computed = cur_n;
        }

        /* copy nxt into cur for next iteration */
        for (int i = 0; i < DIM; ++i)
            for (int j = 0; j < DIM; ++j)
                for (int k = 0; k < MAXSYM; ++k)
                    cur[i][j][k] = nxt[i][j][k];

        cur_len = len;
    }

    return cache[(size_t)n];
}
