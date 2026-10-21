/*
 * A273313  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 643", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*4^n - 4*2^n - 7 for n > 1; a(0) = 1, a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)5;
    seq_t pow4 = pow_seq((seq_t)4, n);   /* 4^n */
    seq_t pow2 = pow_seq((seq_t)2, n);   /* 2^n */
    return (seq_t)4 * pow4 - (seq_t)4 * pow2 - (seq_t)7;
}
