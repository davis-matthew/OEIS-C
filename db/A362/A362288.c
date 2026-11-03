/*
 * A362288  a(n) = Product_{k=0..n} binomial(n,k)^k.
 * https://oeis.org/A362288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} binomial(n,k)^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 23 terms through n = 22 (2026-11-01)
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
    seq_t product = 1;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t binom = binomial(n, k);                     /* binomial(n,k) */
        seq_t power = ipow(binom, (seq_t)k);           /* binom^k   */
        product = seq_mul(product, power);                /* accumulate product */
    }
    return product;
}
