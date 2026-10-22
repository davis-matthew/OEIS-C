/*
 * A322755  Numerator of expected payoff in the "Guessing Card Colors" game
 *          with a 2n-card deck, using an optimal strategy.
 * https://oeis.org/A322755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the expected payoff n - 1/2 +
 *            2^(2n-1)/C(2n,n), expressed as a fraction in lowest terms
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) -> seq_t          */
#include "../utils/pow2.h"    /* pow2(idx_t exp) -> seq_t                */
#include "../utils/gcd.h"    /* gcd(seq_t a, seq_t b) -> seq_t          */

seq_t a(idx_t n)
{
    /* n ≥ 1 in the OEIS sequence; for safety return 0 for n == 0. */
    if (n == 0) return 0;

    /* 2n */
    idx_t two_n = n << 1;

    /* C(2n,n) */
    seq_t bin = binom(two_n, n);

    /* 2^(2n) */
    seq_t pow2v = pow2(two_n);

    /* numerator = (2n-1)*C(2n,n) + 2^(2n) */
    seq_t num = ((seq_t)(two_n - 1)) * bin + pow2v;

    /* denominator = 2*C(2n,n) */
    seq_t den = 2 * bin;

    /* Reduce the fraction to lowest terms. */
    seq_t g = gcd(num, den);

    return num / g;  /* resulting numerator */
}
