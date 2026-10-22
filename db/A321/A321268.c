/*
 * A321268  Number of permutations on [n] whose up-down signature has
 *          nonnegative partial sums and which have exactly two descents.
 * https://oeis.org/A321268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 3*n*2^(n-1) - 2*binomial(n,3) + 4*binomial(n,2) - 1
 *            (For n ≤ 4: a(n) = 0)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* sequence offset: a(1)=a(2)=a(3)=a(4)=0 */
    if (n <= 4) {
        return 0;
    }

    /* 3^n */
    seq_t pow3n = pow3(n);

    /* 2^(n-1) */
    seq_t pow2n1 = pow2(n - 1);

    /* 3*n*2^(n-1) */
    seq_t t = pow2n1;
    t *= (seq_t)n * (seq_t)3;

    /* binom(n,3) and binom(n,2) */
    seq_t bin3 = binomial(n, 3);
    seq_t bin2 = binomial(n, 2);

    /* result = 3^n - 3*n*2^(n-1) - 2*binom(n,3) + 4*binom(n,2) - 1 */
    seq_t result = pow3n;
    result -= t;
    result -= (seq_t)2 * bin3;
    result += (seq_t)4 * bin2;
    result -= 1;

    return result;
}
