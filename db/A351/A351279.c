/*
 * A351279  a(n) = Sum_{k=0..n} 2^k * k^(n-k).
 * https://oeis.org/A351279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 2^k * k^(n-k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 519 (2026-11-01)
 * Helpers:   ipow, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/ipow_idx.h"

/* Compute the n‑th term of OEIS sequence A351279 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t pk;
        if (k == 0)            /* 0^0 = 1, 0^m (m>0) = 0 */
            pk = (n == 0) ? (seq_t)1 : (seq_t)0;
        else
            pk = ipow((seq_t)k, (seq_t)(n - k));

        seq_t pow2 = ipow_idx((seq_t)2, k);
        seq_t term = pk * pow2;
        sum += term;
    }
    return sum;
}
