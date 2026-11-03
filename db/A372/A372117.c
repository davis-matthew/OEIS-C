/*
 * A372117  a(n) = Product_{k=0..n} binomial(n+k, k)^k.
 * https://oeis.org/A372117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=0..n} binomial(n+k, k)^k
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-01)
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
    seq_t result = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin = binomial(n + k, k);
        if (k == 0u)          /* term = 1 */
            continue;
        seq_t term = ipow(bin, (seq_t)k);
        result = seq_mul(result, term);
    }
    return result;
}
