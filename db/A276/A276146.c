/*
 * A276146  a(n) = A034968(A225901(n)).
 * https://oeis.org/A276146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A034968(A225901(n)); with factorial-base digits d_k of n
 *            (0<=d_k<=k, weight k!), a(n) = sum over nonzero d_k of (k+1 -
 *            d_k). a(0)=0
 * Range:     u8 n <= 40320, u16 n <= 40320, u32 n <= 40320, u64 n <= 40320,
 *            u128 n <= 40320, big any n
 * Validated: standard, pass, 137 terms through n = 8143 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t r = 2;
    while (n > 0) {
        idx_t d = n % r;
        if (d != 0) sum += (seq_t)(r - d);
        n /= r;
        r++;
    }
    return sum;
}
