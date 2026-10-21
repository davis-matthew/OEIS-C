/*
 * A255373  Number of strings of n decimal digits that contain at least one
 *          string of exactly 3 consecutive "0" digits.
 * https://oeis.org/A255373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=a(1)=a(2)=0, a(3)=1
 * For n>=4: a(n)=9*(10^(n-4)-a(n-4)+∑_{i=3}^{n-1} a(i))
 * This implementation uses an automaton DP that keeps track of the
 * last up to 4 zeros and whether an isolated run of three zeros has
 * already appeared.  The DP works in O(n) time and uses only constant
 * extra memory besides a cache of computed values.
 */
#include <stdlib.h>

static seq_t *seq_cache = NULL;      /* cache of a(n) */
static size_t cache_capacity = 0;    /* allocated size */
static idx_t cache_last = 0;         /* highest index computed */

/* DP state for the most recently computed n (cache_last) */
static seq_t dp_u0 = 1, dp_u1 = 0, dp_u2 = 0, dp_u3 = 0, dp_u4 = 0, dp_v = 0;

/* compute values up to n */
static void ensure_cache(idx_t n)
{
    if (!seq_cache) {
        cache_capacity = 1;
        seq_cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!seq_cache) return;          /* allocation failed – silently fail */
        seq_cache[0] = 0;
        /* dp_state already initialized to values for n=0 */
        cache_last = 0;
    }
    while (cache_last < n) {
        seq_t next_u0 = (seq_t)9 * (dp_u0 + dp_u1 + dp_u2 + dp_u4);
        seq_t next_u1 = dp_u0;
        seq_t next_u2 = dp_u1;
        seq_t next_u3 = dp_u2;
        seq_t next_u4 = dp_u3 + dp_u4;
        seq_t next_v  = (seq_t)9 * dp_u3 + dp_v * (seq_t)10;

        dp_u0 = next_u0;
        dp_u1 = next_u1;
        dp_u2 = next_u2;
        dp_u3 = next_u3;
        dp_u4 = next_u4;
        dp_v  = next_v;

        idx_t i = cache_last + 1;
        if ((size_t)i >= cache_capacity) {
            size_t new_cap = i + 1;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) return; /* memory error; silently leave cache incomplete */
            seq_cache = tmp;
            cache_capacity = new_cap;
        }
        seq_cache[i] = dp_v + dp_u3;    /* a(i) = seen + last run of 3 zeros */
        cache_last = i;
    }
}

/* sequence function required by the harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_capacity) return seq_cache[n];
    /* if allocation failed, return 0 */
    return 0;
}
