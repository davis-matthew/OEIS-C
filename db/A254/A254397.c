/*
 * A254397  Initial digits of A237424 in decimal representation.
 * https://oeis.org/A254397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000030(A237424(n)), where A237424 lists (10^a+10^b+1)/3,
 *            a>=b>=0, by a then b; computed from the (a,b) block index:
 *            leading digit is 1 for a=0, 4 or 7 for a=1, else 3 if b<a and 6
 *            if b=a
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9847 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t aa = 0;
    idx_t cumulative = 0;
    while (cumulative + (aa + 1) < n) {
        cumulative += aa + 1;
        ++aa;
    }
    idx_t b = n - cumulative - 1;

    if (aa == 0)
        return 1;
    if (aa == 1)
        return (b == 0) ? 4 : 7;
    return (b == aa) ? 6 : 3;
}
