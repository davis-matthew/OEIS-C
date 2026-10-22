/*
 * A345035  a(n) = Sum_{k=1..n} (-3)^(floor(n/k) - 1).
 * https://oeis.org/A345035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-3)^(floor(n/k)-1)
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 40, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety for indexes below offset */

    /* Allocate an array of powers of -3 up to exponent n-1 */
    size_t sz = (size_t)n;                /* for n up to about 10 000 this is safe */
    seq_t *powers = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!powers) return (seq_t)0;          /* fallback on allocation failure */

    /* powers[0] = (-3)^0 = 1 ; powers[i] = (-3)^i */
    powers[0] = (seq_t)1;
    const seq_t mult = (seq_t)(-3);
    for (size_t i = 1; i < sz; ++i) {
        powers[i] = powers[i - 1] * mult;
    }

    /* Sum over k = 1 … n */
    seq_t sum = (seq_t)0;
    for (size_t k = 1; k <= sz; ++k) {
        /* floor(n / k) - 1 gives an index in [0, n-1] */
        size_t exp_idx = (size_t)(n / (idx_t)k) - 1;
        sum += powers[exp_idx];
    }

    free(powers);
    return sum;
}
