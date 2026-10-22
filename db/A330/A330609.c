/*
 * A330609  T(n, k) = binomial(n-k-1, k-1)*(n-k)!/k! for n >= 0 and 0 <= k <=
 *          floor(n/2). Irregular triangle read by rows.
 * https://oeis.org/A330609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = binomial(n-k-1, k-1) * (n-k)! / k! Sequence a(n) is the
 *            triangle read by rows. a(0) = 1, a(1) = 0, a(2) = 0, a(3) = 1, …
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 56, u64 n <= 132,
 *            u128 n <= 342, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* linear store of all a(n) values */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t cache_size = 0;         /* number of values stored in cache */

static seq_t *row_prevprev = NULL;   /* row n-2 */
static seq_t *row_prev    = NULL;    /* row n-1 */
static size_t prevprev_len = 0;
static size_t prev_len    = 0;

static idx_t current_row = 0;        /* last row that has been built */

/* Grow the cache so that it can hold at least (n+1) elements. */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while ((size_t)n + 1 > new_cap)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache     = new_cache;
    cache_cap = new_cap;
}

/* Compute the triangle row by row up to the desired index n. */
static void build_to_index(idx_t n)
{
    /* Initialise the very first row (n = 0) on first call. */
    if (cache_size == 0) {
        ensure_cache_cap(0);
        cache[0] = (seq_t)1;         /* T(0,0) = 1 */
        cache_size = 1;

        /* allocate storage for row 0 */
        row_prev    = (seq_t *)malloc(1 * sizeof(seq_t));
        row_prevprev = NULL;
        row_prev[0] = (seq_t)1;
        prev_len    = 1;
        prevprev_len = 0;
        current_row = 0;
    }

    /* Build rows until the cache contains all values up to index n. */
    while (cache_size <= n) {
        /* advance to the next row */
        ++current_row;
        size_t cur_len = current_row / 2 + 1;

        seq_t *new_row = (seq_t *)malloc(cur_len * sizeof(seq_t));

        for (size_t k = 0; k < cur_len; ++k) {
            seq_t val;
            if (k == 0) {
                val = (seq_t)0;               /* T(n,0) = 0 for n>0 */
            } else {
                seq_t term1 = (seq_t)0;
                if (k < prev_len) {
                    /* term1 = (n-1) * T(n-1,k) */
                    term1 = row_prev[k] * (seq_t)(current_row - 1);
                }
                seq_t term2 = (seq_t)0;
                if (k - 1 < prevprev_len) {
                    term2 = row_prevprev[k - 1];  /* T(n-2,k-1) */
                }
                val = term1 + term2;
            }
            new_row[k] = val;
            ensure_cache_cap(cache_size);
            cache[cache_size++] = val;
        }

        /* advance the row pointers */
        free(row_prevprev);
        row_prevprev = row_prev;
        prevprev_len = prev_len;
        row_prev    = new_row;
        prev_len    = cur_len;
    }
}

seq_t a(idx_t n)
{
    build_to_index(n);
    return cache[n];
}
