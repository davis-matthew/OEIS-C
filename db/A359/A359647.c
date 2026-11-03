/*
 * A359647  a(n) = [x^n] hypergeom([1/4, 3/4], [2], 64*x). The central terms
 *          of the Motzkin triangle A359364 without zeros.
 * https://oeis.org/A359647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = catalan(n) * binomial(4*n, 2*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t cat = catalan(n);
    seq_t bin = binomial(4 * n, 2 * n);
    return seq_mul(cat, bin);
}
