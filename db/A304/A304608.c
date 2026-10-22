/*
 * A304608  a(n) = 288*2^n + 178 (n >= 1).
 * https://oeis.org/A304608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 288*2^n + 178 (n >= 1)
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 2^exp in seq_t using exponentiation by squaring.
 * Works for all supported seq_t types (fixed width and arbitrary precision). */
static seq_t pow2_exp(idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* External linkage function requested by the build. */
seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, return 0 as safe fallback */

    seq_t pow2n = pow2_exp(n);
    seq_t res   = pow2n * (seq_t)288;
    res += (seq_t)178;
    return res;
}
