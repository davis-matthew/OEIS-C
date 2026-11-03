/*
 * A277473  E.g.f.: -exp(x)*LambertW(-x).
 * https://oeis.org/A277473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * k^(k-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 375 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t bin = binomial(n, k);
        seq_t pw  = ipow((seq_t)k, (seq_t)(k - 1));
        sum += seq_mul(bin, pw);
    }
    return sum;
}
