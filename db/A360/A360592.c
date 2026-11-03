/*
 * A360592  G.f.: Sum_{k>=0} (1 + k*x)^k * x^k.
 * https://oeis.org/A360592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n-k,k) * (n-k)^k
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 29, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 723 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t max_k = n >> 1;            /* floor(n/2) */
    for (idx_t k = (idx_t)0; k <= max_k; ++k)
    {
        seq_t bin = binomial(n - k, k);
        seq_t pw  = ipow_i32((seq_t)(n - k), (int)k);
        sum += bin * pw;
    }
    return sum;
}
