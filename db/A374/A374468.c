/*
 * A374468  Parity of the digit sum of n in factorial base.
 * https://oeis.org/A374468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = parity (even=0, odd=1) of the digit sum of n in factorial
 *            base
 * Range:     u8 n <= 9917, u16 n <= 9917, u32 n <= 9917, u64 n <= 9917,
 *            u128 n <= 9917, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to factorial base and accumulate digit parity.
       In factorial base: n = Σ d_k * k! with 0 ≤ d_k ≤ k.
       The standard conversion uses successive divisions by (k+1):
           k = 1:  d_1 = n % 2;  n /= 2;
           k = 2:  d_2 = n % 3;  n /= 3;
           ...
       The sum parity can be updated incrementally. */
    idx_t rem = n;
    idx_t denom = 2;          /* corresponds to k+1 for current digit */
    int parity = 0;           /* 0 = even, 1 = odd */

    while (rem != 0) {
        idx_t digit = rem % denom;
        parity ^= (int)(digit & 1);   /* add parity of this digit */
        rem /= denom;
        denom++;                       /* next divisor (k+1) */
    }

    return (seq_t)parity;
}
