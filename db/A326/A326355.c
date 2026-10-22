/*
 * A326355  Number of permutations of length n with at most two descents.
 * https://oeis.org/A326355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - n*2^n + n^2/2 - n/2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: fast exponentiation for unsigned integers.
   Computes base^exp using binary exponentiation. */
static inline seq_t pow_uint(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp) {
        if (exp & (idx_t)1)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t pow3n = pow_uint((seq_t)3, n);
    seq_t pow2n = pow_uint((seq_t)2, n);

    seq_t mult = (seq_t)n * pow2n;
    seq_t res = pow3n - mult;

    seq_t nn = (seq_t)n * n;
    seq_t sub = nn - (seq_t)n;
    seq_t div = sub / (seq_t)2;

    res += div;
    return res;
}
