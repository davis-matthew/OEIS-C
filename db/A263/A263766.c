/*
 * A263766  a(n) = Product_{k=1..n} (k^2 - 2).
 * https://oeis.org/A263766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{k=1..n} (k^2 - 2)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n) = (n^2 - 2) * a(n‑1) */

#include <stddef.h>   /* for idx_t, seq_t definition in build */

#define MAX_CACHE 10001          /* cache size for n <= 10000 */

static seq_t cache[MAX_CACHE];  /* memoised products */
static idx_t cached = 0;        /* number of entries currently stored */

/* Compute a(n) : product of (k^2-2) for k=1..n */
seq_t a(idx_t n)
{
    /* Handle the first term explicitly. */
    if (cached == 0) {
        cache[0] = (seq_t)1;
        cached   = 1;
    }

    /* Use memoised table for n < MAX_CACHE */
    if (n < MAX_CACHE) {
        if (n >= cached) {               /* grow cache up to n */
            for (idx_t i = cached; i <= n; ++i) {
                seq_t term = (seq_t)i * (seq_t)i - (seq_t)2;
                cache[i] = cache[i - 1] * term;
            }
            cached = n + 1;
        }
        return cache[n];
    }

    /* For n >= MAX_CACHE, compute on the fly (no memoisation) */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = (seq_t)i * (seq_t)i - (seq_t)2;
        result = result * term;
    }
    return result;
}
