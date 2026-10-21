/*
 * A269584  Number of length-4 0..n arrays with no repeated value differing
 *          from the previous repeated value by more than one.
 * https://oeis.org/A269584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 4*n^3 + 5*n^2 + 5*n + 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn * nn * nn
         + 4 * nn * nn * nn
         + 5 * nn * nn
         + 5 * nn
         + 1;
}
