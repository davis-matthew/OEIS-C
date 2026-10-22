/*
 * A309256  a(n) = n + 1 - Sum_{k=0..n} (Stirling2(n,k) mod 2).
 * https://oeis.org/A309256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 1 - Sum_{k=0..n} (Stirling2(n,k) mod 2)
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

static seq_t *a_cache   = NULL;   /* cached a(n) values */
static size_t cache_cap = 0;      /* capacity of a_cache */
static idx_t  max_n     = 0;      /* last n that was computed */

static unsigned char *prev_row = NULL; /* parity bits of previous row */
static size_t        prev_len = 0;      /* length of prev_row */

/* enlarge a_cache to at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = seq_realloc(a_cache, cache_cap, new_cap);
    a_cache = tmp;
    cache_cap = new_cap;
}

/* initialise static variables on first use */
static void init(void)
{
    /* cache for a(n) */
    cache_cap = 1;
    a_cache   = seq_realloc(NULL, 0, 1);
    a_cache[0] = (seq_t)0;          /* a(0) = 0 */

    /* first row of the Stirling–type triangle: S(0,0) = 1 (odd) */
    prev_len  = 1;
    prev_row  = (unsigned char *)malloc(prev_len);
    prev_row[0] = 1;                 /* S(0,0) is odd */

    max_n = 0;
}

/* compute a(n) for any n >= 0, in any order of calls */
seq_t a(idx_t n)
{
    if (a_cache == NULL)
        init();

    /* value already known? */
    if (n <= max_n)
        return a_cache[n];

    /* extend cache if necessary */
    ensure_cache_cap((size_t)n + 1);

    /* compute rows from max_n+1 up to n */
    for (idx_t m = max_n + 1; m <= n; ++m)
    {
        size_t cur_len = m + 1;
        unsigned char *cur_row = (unsigned char *)malloc(cur_len);
        cur_row[0] = 0;                    /* S(m,0) = 0 (even) */

        uint64_t odd_cnt = 0;

        /* compute parity for k = 1 .. m */
        for (idx_t k = 1; k <= m; ++k)
        {
            unsigned char val = prev_row[k - 1];   /* S(m-1,k-1) */

            /* add (k odd ? S(m-1,k) : 0)   */
            if ((k & 1u) && (k < (idx_t)prev_len))
                val ^= prev_row[k];

            cur_row[k] = val;
            odd_cnt += val;
        }

        /* a(m) = (m+1) - (# odd entries) */
        a_cache[m] = (seq_t)(m + 1) - (seq_t)odd_cnt;

        /* free previous row and promote current row */
        free(prev_row);
        prev_row = cur_row;
        prev_len = cur_len;
    }

    max_n = n;
    return a_cache[n];
}
