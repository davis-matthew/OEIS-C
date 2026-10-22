/*
 * A322756  Denominator of expected payoff in the "Guessing Card Colors" game
 *          with a 2n-card deck, using an optimal strategy.
 * https://oeis.org/A322756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of n - 1/2 + 2^(2n-1) / binomial(2n,n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, gcd, mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/gcd.h"
#include "../utils/mul_small.h"

/* External linkage function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, safe guard */

    /* 2*n and 2*n-1 as idx_t */
    idx_t twice_n       = 2 * n;
    idx_t twice_n_minus = twice_n - 1U;

    /* binomial coefficient C(2n,n) */
    seq_t C = binomial(twice_n, n);

    /* power T = 2^(2n-1) */
    seq_t T = pow2(twice_n_minus);

    /* numerator of the fraction before reduction:
       A = (2n-1) * C + 2 * T   (2n-1 fits in unsigned int) */
    seq_t part1 = mul_small(C, (unsigned)(twice_n_minus));
    seq_t part2 = mul_small(T, 2);
    seq_t A     = part1 + part2;

    /* common denominator before reduction: B = 2 * C */
    seq_t B = mul_small(C, 2);

    /* greatest common divisor of A and B */
    seq_t g = gcd(A, B);

    /* reduced denominator: B / g */
    seq_t result = B / g;

    return result;
}
