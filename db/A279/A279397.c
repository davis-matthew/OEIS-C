/*
 * A279397  Row sums of triangle A279396.
 * https://oeis.org/A279397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=1..n} Sum_{d|m} (-1)^(m-d) * d^(n-m)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 28, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-08)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pos = 0;
    seq_t neg = 0;
    for (idx_t m = 1; m <= n; ++m) {
        idx_t k = n - m;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d != 0)
                continue;
            seq_t v = ipow_idx((seq_t)d, k);
            if (((m - d) & 1) == 0)
                pos += v;
            else
                neg += v;
            idx_t e = m / d;
            if (e != d) {
                seq_t v2 = ipow_idx((seq_t)e, k);
                if (((m - e) & 1) == 0)
                    pos += v2;
                else
                    neg += v2;
            }
        }
    }
    seq_t result = pos;
    result -= neg;
    return result;
}
