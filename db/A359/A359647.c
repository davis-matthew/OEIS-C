/*
 * A359647  a(n) = [x^n] hypergeom([1/4, 3/4], [2], 64*x). The central terms
 *          of the Motzkin triangle A359364 without zeros.
 * https://oeis.org/A359647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = catalan(n) * binomial(4*n, 2*n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, catalan, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t cat = catalan(n);
    seq_t bin = binomial(4 * n, 2 * n);
    return seq_mul(cat, bin);
}
