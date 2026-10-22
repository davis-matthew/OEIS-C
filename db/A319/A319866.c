/*
 * A319866  a(n) = 2*1 + 4*3 + 6*5 + 8*7 + 10*9 + 12*11 + ... + (up to the
 *          n-th term).
 * https://oeis.org/A319866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 3*n^2 - 2*n) / 12 for even n a(n) = (2*n^3 -
 *            3*n^2 + 10*n + 15) / 12 for odd n
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe fallback */

    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) { /* even n */
        return (( (seq_t)2 * nn * nn * nn ) +
                ( (seq_t)3 * nn * nn ) -
                ( (seq_t)2 * nn )) / (seq_t)12;
    } else {            /* odd n */
        return (( (seq_t)2 * nn * nn * nn ) -
                ( (seq_t)3 * nn * nn ) +
                ( (seq_t)10 * nn ) +
                (seq_t)15 ) / (seq_t)12;
    }
}
