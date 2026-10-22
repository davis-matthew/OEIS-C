/*
 * A383201  Number of positive integers with n digits in which adjacent digits
 *          differ by at most 6.
 * https://oeis.org/A383201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Number of n‑digit positive integers whose adjacent digits differ
 *            * by at most 6 (OEIS A383201). The computation uses a simple DP
 *            over the * last digit, storing results in a static cache for
 *            repeated calls
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types are supplied by the build system.  Do not redefine them. */

static seq_t *cache_seq = NULL;        /* a(n) values, 1‑based indexing */
static idx_t  cache_len = 0;           /* allocated size of cache_seq   */
static idx_t  cached_max = 0;          /* largest n whose value is known */
static seq_t  cur_state[10] = {0};     /* digit counts for the current length */

/* --------------------------------------------------------------------- */
/*  seq_t a(idx_t n)                                                     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; n==0 unused */

    /* Expand the cache if the requested index is beyond what's allocated. */
    if ((idx_t)(n + 1) > cache_len) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache_seq, new_cap * sizeof(seq_t));
        cache_seq = tmp;
        cache_len = (idx_t)new_cap;
    }

    /* Return already computed value if available. */
    if (n <= cached_max) return cache_seq[n];

    /* Initialise the DP for length 1 if this is the first call. */
    if (cached_max == 0) {
        for (int d = 0; d < 10; ++d) cur_state[d] = 0;
        for (int d = 1; d <= 9; ++d) cur_state[d] = 1;  /* one‑digit numbers */
        seq_t total = 0;
        for (int d = 0; d < 10; ++d) total += cur_state[d];
        cache_seq[1] = total;
        cached_max   = 1;
    }

    /* Temporary array for the next state. */
    seq_t nxt[10];

    /* Extend the DP from cached_max+1 up to the requested n. */
    for (idx_t len = cached_max + 1; len <= n; ++len) {
        /* Clear the next state. */
        for (int d = 0; d < 10; ++d) nxt[d] = 0;

        /* Transition from cur_state to nxt, respecting |prev - next| <= 6. */
        for (int prev = 0; prev < 10; ++prev) {
            if (cur_state[prev] == 0) continue;
            int low  = prev - 6;
            if (low  < 0) low  = 0;
            int high = prev + 6;
            if (high > 9) high = 9;
            for (int d = low; d <= high; ++d) {
                nxt[d] += cur_state[prev];
            }
        }

        /* Compute the total for this length and update cur_state. */
        seq_t total = 0;
        for (int d = 0; d < 10; ++d) {
            total += nxt[d];
            cur_state[d] = nxt[d];
        }
        cache_seq[len] = total;
    }

    cached_max = n;
    return cache_seq[n];
}
