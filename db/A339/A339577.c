/*
 * A339577  a(n) = product of nonzero entries in row n of A235791.
 * https://oeis.org/A339577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..floor((sqrt(8n+1)-1)/2)} ceil((n+1)/k -
 *            (k+1)/2), the product of the entries in row n of A235791 (all
 *            entries are positive)
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-08)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t s = isqrt(8 * n + 1);
    idx_t kmax = (s - 1) / 2;
    seq_t prod = 1;
    for (idx_t k = 1; k <= kmax; k++) {
        seq_t kk = (seq_t)k;
        seq_t num = 2 * ((seq_t)n + 1) - kk * (kk + 1);
        seq_t den = 2 * kk;
        seq_t val = (num + den - 1) / den;
        if (val != 0) prod *= val;
    }
    return prod;
}
