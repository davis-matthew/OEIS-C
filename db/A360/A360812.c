/*
 * A360812  Expansion of Sum_{k>=0} ( x / (1 - (k * x)^2) )^k.
 * https://oeis.org/A360812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (n-2*k)^(2*k) * binomial(n-k-1,k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/seq_pow.h"    /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/seq_mul.h"    /* seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* special case: 0^0 * binomial(-1,0) = 1 */

    seq_t sum = (seq_t)0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t base = (seq_t)(n - 2 * k);              /* base fits in seq_t */
        seq_t pow = seq_pow(base, (idx_t)(2 * k));    /* (n-2k)^(2k) */
        seq_t comb = binomial(n - k - 1, k);          /* C(n-k-1, k) */
        seq_t term = seq_mul(pow, comb);              /* term = pow * comb */
        sum += term;
    }
    return sum;
}
