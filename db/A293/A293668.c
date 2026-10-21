/*
 * A293668  First differences of A292046.
 * https://oeis.org/A293668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n)=a(log2(n))+1 if n is a power of 2,
 *            otherwise a(n)=1
 * Range:     u8 n <= 9477, u16 n <= 9477, u32 n <= 9477, u64 n <= 9477,
 *            u128 n <= 9477, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;      /* base case */
    }
    /* If n is not a power of two, the value is 1. */
    if ((n & (n - 1)) != 0) {
        return (seq_t)1;
    }
    /* For powers of two, count the number of times the power-of-two
       recursive rule applies until reaching 0.  The result is 1 plus
       that count, which equals the value of a(n). */
    seq_t result = (seq_t)1;  /* value of a(0) */
    idx_t m = n;
    /* Iterate while m is a power of two.  __builtin_ctzll gives log2(m). */
    while (m > 0 && (m & (m - 1)) == 0) {
        result++;                            /* account for one recursive step */
        m = (idx_t)__builtin_ctzll((unsigned long long)m);
    }
    return result;
}
