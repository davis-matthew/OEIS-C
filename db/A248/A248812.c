/*
 * A248812  Repeated terms of (2n)! (A010050).
 * https://oeis.org/A248812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*floor(n/2))!
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of A248812, which is the factorial of the
 * largest even integer less than or equal to n.  In other words,
 * a(n) = (2*floor(n/2))!.
 *
 * The implementation multiplies successively from 2 up to the desired
 * even value, using seq_t arithmetic which may be a builtin unsigned
 * type or an arbitrary‑precision integer class.
 */
seq_t a(idx_t n)
{
    /* Determine the smallest even integer <= n: */
    idx_t even = (n & 1) ? n - 1 : n;      /* even = 2 * floor(n/2) */

    seq_t result = 1;
    for (idx_t i = 2; i <= even; ++i) {
        result *= (seq_t)i;
    }
    return result;
}
