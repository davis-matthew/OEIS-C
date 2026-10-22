/*
 * A348308  a(n) = Sum_{k=0..floor(n/6)} (-1)^k * binomial(n-3*k,3*k).
 * https://oeis.org/A348308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) - a(n-6) initial values:
 *            a(0)=1,a(1)=1,a(2)=1,a(3)=1,a(4)=1,a(5)=1,a(6)=0
 * Range:     i8 n <= 13, i16 n <= 28, i32 n <= 51, i64 n <= 104,
 *            i128 n <= 206, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t */

static seq_t cache[10006];      /* at least 0..10000, plus 6 for a(6) */
static size_t computed_max = 0;
static int initialized = 0;

seq_t a(idx_t n) {
    /* -- one‑time initialization of the small known values -- */
    if (!initialized) {
        /* a(0) … a(5) = 1;  a(6) = 0 */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 1;
        cache[5] = 1;
        cache[6] = 0;
        computed_max = 6;
        initialized = 1;
    }

    /* If the requested index is already known, return it. */
    if ((size_t)n <= computed_max) {
        return cache[n];
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (size_t i = computed_max + 1; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = 3*a(i-1) - 3*a(i-2) + a(i-3) - a(i-6) */
        cache[i] = 3 * cache[i-1] - 3 * cache[i-2] + cache[i-3] - cache[i-6];
    }
    computed_max = (size_t)n;
    return cache[n];
}
