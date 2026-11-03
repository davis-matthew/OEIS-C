/*
 * A377146  a(n) = Sum_{k=0..floor(n/2)} binomial(k+2,2) *
 *          binomial(k,n-2*k)^2.
 * https://oeis.org/A377146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(k+2,2)*binomial(k,n-2k)^2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; (k << 1) <= n; ++k) {
        idx_t r = n - (k << 1);
        if (r > k) continue;            /* binomial(k,r) would be 0 */
        seq_t c1 = binomial(k + 2, 2);
        seq_t c2 = binomial(k, r);
        sum += c1 * c2 * c2;
    }
    return sum;
}
