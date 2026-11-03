/*
 * A277456  a(n) = 1 + Sum_{k=1..n} binomial(n,k) * 3^k * k^k.
 * https://oeis.org/A277456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n} binomial(n,k) * 3^k * k^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 326 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helpers */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)1;          /* start with the leading +1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t bin = binomial(n, k);          /* C(n,k) */
        seq_t p3  = pow3(k);                 /* 3^k */
        seq_t pk  = ipow_u64_idx((uint64_t)k, k);/* k^k */

        seq_t term = bin * p3;
        term = term * pk;
        sum = sum + term;
    }
    return sum;
}
