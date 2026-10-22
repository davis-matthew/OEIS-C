/*
 * A354918  a(n) = A344005(n) mod 2, where A344005(n) is the smallest positive
 *          m such that n divides the oblong number m*(m+1).
 * https://oeis.org/A354918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9523, u16 n <= 9523, u32 n <= 9523, u64 n <= 9523,
 *            u128 n <= 9523, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as defined in OEIS A354918:
   a(n) = parity of the smallest positive m such that n | m*(m+1).
   The function searches for that m by brute force and returns 0 or 1. */
#include <stdint.h>   /* for uint64_t and related types */

seq_t a(idx_t n)
{
    for (idx_t m = 1; m <= n; ++m) {
        idx_t aa = m % n;
        idx_t bb = (m + 1) % n;
        /* m*(m+1) % n == 0? */
        if (((aa * bb) % n) == 0) {
            return (seq_t)(m & 1ULL);  /* parity of m */
        }
    }
    /* Should never reach here because m = n always satisfies n | n*(n+1). */
    return (seq_t)(n & 1ULL);
}
