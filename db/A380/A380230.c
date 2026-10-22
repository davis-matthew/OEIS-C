/*
 * A380230  a(n) = n*A070939(n).
 * https://oeis.org/A380230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * (floor(log2(n)) + 1) for n > 0; a(0) = 0
 * Range:     u8 n <= 42, u16 n <= 5041, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is n * A070939(n) where A070939(n) = number of binary digits of n. */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* Compute number of bits in n. */
    unsigned long long nn = (unsigned long long)n;
    /* __builtin_clzll is defined for unsigned long long. */
    unsigned bits = 64 - __builtin_clzll(nn);
    return (seq_t)n * (seq_t)bits;
}
