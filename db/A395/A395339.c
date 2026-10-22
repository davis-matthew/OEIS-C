/*
 * A395339  a(n) = (2*n)! * [x^(2*n)] cosh(x) * cos(x)^2.
 * https://oeis.org/A395339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A395339.
   Formula: a(n) = (Re((1+2*i)^(2*n)) + 1)/2
   Recurrence: a(0)=1, a(1)=-1, a(2)=-3,
                a(n) = -5*a(n-1) - 19*a(n-2) + 25*a(n-3). */

#include <stddef.h>

/* Cache for computed terms. Size 10001 covers all indices up to 10000
   which is sufficient for the expected usage.  */
static seq_t cache[10001];
static size_t computed = 0;   /* number of terms currently stored */

/* Return a(n) for n ≥ 0. */
seq_t a(idx_t n) {
    /* Exit early if the requested index is out of the preset range. */
    if (n >= 10001) {
        /* The sequence is defined only for indices up to 10000 in the
           required tests.  For higher indices we simply abort.  */
        abort();
    }

    /* Initialize the first three terms once. */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-3;
        computed = 3;
    }

    /* If needed, extend the cache up to the requested index. */
    if (n >= computed) {
        for (size_t i = computed; i <= (size_t)n; ++i) {
            cache[i] = -(seq_t)5 * cache[i-1]
                       - (seq_t)19 * cache[i-2]
                       + (seq_t)25 * cache[i-3];
        }
        computed = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
