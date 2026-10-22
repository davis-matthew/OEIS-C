/*
 * A387696  a(n) = Sum_{k=0..floor(n/4)} 2^(n-4*k) *
 *          binomial(2*n-6*k+1,2*k+1).
 * https://oeis.org/A387696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387696
   a(0) = 1
   a(1) = 6
   a(2) = 20
   a(3) = 56
   a(4) = 145
   a(5) = 372
   a(6) = 972
   a(7) = 2592
   Recurrence:
   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-4) + 4*a(n-5) - a(n-8)  (n ≥ 8)
*/
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t computed_up_to = 0;
static bool initialized = false;
static const seq_t init_vals[8] = {1, 6, 20, 56, 145, 372, 972, 2592};

seq_t a(idx_t n)
{
    /* Ensure cache has room for n+1 entries */
    if (n >= cache_size) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 8) new_cap = 8;            /* at least capacity for base terms */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_size = new_cap;
    }

    /* One‑time initialization of the base terms */
    if (!initialized) {
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init_vals[i];
        computed_up_to = 7;
        initialized = true;
    }

    /* Compute any missing terms */
    if ((size_t)n > computed_up_to) {
        size_t start = computed_up_to + 1;
        if (start < 8) start = 8;   /* recurrence valid only for n ≥ 8 */
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t val =
                4 * cache[i - 1]          /* 4*a(i-1) */
                - 4 * cache[i - 2]        /* -4*a(i-2) */
                + 2 * cache[i - 4]        /* +2*a(i-4) */
                + 4 * cache[i - 5]        /* +4*a(i-5) */
                - cache[i - 8];           /* -a(i-8) */
            cache[i] = val;
        }
        computed_up_to = (size_t)n;
    }

    return cache[(size_t)n];
}
