/*
 * A352994  Number of copies of the star graph S(2,2,1) contained within the
 *          n-dimensional hypercube graph.
 * https://oeis.org/A352994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n) * (n choose 3) * 3n * (n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    seq_t pow2n  = pow2(n);          /* 2^n */
    seq_t comb   = binomial(n, 3);   /* C(n,3) */
    seq_t factor = (seq_t)3 * (seq_t)n * (seq_t)(n - 2);
    return pow2n * comb * factor;
}
