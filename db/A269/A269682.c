/*
 * A269682  Number of length-7 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than plus or minus one
 *          modulo n+1.
 * https://oeis.org/A269682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 6*n^5 + 20*n^4 + 8*n^3 + 10*n^2 + 12*n - 10
 *            for n > 1, a(1)=30
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return 30;
    seq_t N = (seq_t)n;
    seq_t t = (seq_t)1;
    t = t * N + 7;
    t = t * N + 6;
    t = t * N + 20;
    t = t * N + 8;
    t = t * N + 10;
    t = t * N + 12;
    t = t * N - 10;
    return t;
}
