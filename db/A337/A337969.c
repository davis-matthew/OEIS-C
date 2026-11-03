/*
 * A337969  a(n) = 3^(n*(n-1)/2) + 3^(n*(n+1)/2) for n > 0, with a(0) = 1.
 * https://oeis.org/A337969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n*(n-1)/2) + 3^(n*(n+1)/2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t nn   = (seq_t)n;
    seq_t exp1 = nn * (nn - 1) / 2;
    seq_t exp2 = nn * (nn + 1) / 2;
    seq_t t1   = ipow((seq_t)3, exp1);
    seq_t t2   = ipow((seq_t)3, exp2);
    return t1 + t2;
}
