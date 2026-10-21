/*
 * A251936  Number of length 2+2 0..n arrays with the sum of the maximum minus
 *          the median of adjacent triples multiplied by some arrangement of
 *          +-1 equal to zero.
 * https://oeis.org/A251936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 14n^3 + 23n^2 + 16n + 6)/6
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Work in 64‑bit unsigned arithmetic to avoid overflow for the
       ranges the tests use (n up to about 10 000).  The result is
       cast back to seq_t, which may be an arbitrary‑precision integer
       class or a smaller integer type that fits the result. */
    uint64_t nn = (uint64_t)n;
    uint64_t n2 = nn * nn;          /* n^2 */
    uint64_t n3 = n2 * nn;          /* n^3 */
    uint64_t n4 = n3 * nn;          /* n^4 */

    uint64_t numerator = n4
                       + (uint64_t)14 * n3
                       + (uint64_t)23 * n2
                       + (uint64_t)16 * nn
                       + (uint64_t)6;

    return (seq_t)(numerator / (uint64_t)6);
}
