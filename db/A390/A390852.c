/*
 * A390852  a(n) = Sum_{k=0..n} (-1)^k * binomial(n+k+1,n-k) * Fibonacci(k+1).
 * https://oeis.org/A390852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - a(n-4) (n ≥ 4) a(0) = 1,
 *            a(1) = 1, a(2) = 1, a(3) = 3 (offset 0)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 59, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define A390852_MAX_N 10000   /* sufficient for n ≤ ~10000 */

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t cache[A390852_MAX_N + 1];
    static size_t computed_upto = 0;   /* highest index computed so far */
    static int initialized        = 0;

    size_t idx = (size_t)n;

    if (!initialized) {
        /* initialise first four values once */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        computed_upto = 3;
        initialized   = 1;
    }

    /* guard against requests beyond the fixed cache size */
    if (idx > A390852_MAX_N) {
        /* for safety return 0; the caller may handle this case */
        return (seq_t)0;
    }

    /* if the term is already known, return it immediately */
    if (idx <= computed_upto) {
        return cache[idx];
    }

    /* compute missing terms up to idx */
    for (size_t i = computed_upto + 1; i <= idx; ++i) {
        seq_t t = 3 * cache[i - 1];
        t      -= 3 * cache[i - 2];
        t      += 3 * cache[i - 3];
        t      -= cache[i - 4];
        cache[i] = t;
    }
    computed_upto = idx;

    return cache[idx];
}
