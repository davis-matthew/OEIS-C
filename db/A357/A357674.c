/*
 * A357674  a(n) = ( Sum_{k = 0..2*n} binomial(n+k-1,k) )^4 * ( Sum_{k =
 *          0..2*n} binomial(n+k-1,k)^2 )^3.
 * https://oeis.org/A357674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( Σ_{k=0..2*n} binomial(n+k-1,k) )^4 * * ( Σ_{k=0..2*n}
 *            binomial(n+k-1,k)^2 )^3
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

/*
 * Computes term a(n) of OEIS sequence A357674.
 *
 * The implementation uses the provided binomial() routine to
 * evaluate S1 = binomial(3n,2n) and iteratively builds the
 * sequence of binomial(n+k-1,k) for k = 0..2n on the fly.
 * No temporary large arrays are needed; the complexity is
 * linear in n, which satisfies the time constraints for
 * all supported build sizes.
 */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* S1 = Σ_{k=0}^{2n} binomial(n+k-1,k) = binomial(3n,2n) */
    idx_t two_n   = n << 1;          /* 2*n */
    idx_t three_n = n + two_n;       /* 3*n */
    seq_t S1 = binomial(three_n, two_n);

    /* S2 = Σ_{k=0}^{2n} binomial(n+k-1,k)^2 */
    seq_t sum_sq = (seq_t)1;         /* term for k = 0 is 1, its square is 1 */
    seq_t term   = (seq_t)1;         /* C(n-1,0) = 1 */

    for (idx_t k = 0; k < two_n; ++k) {
        /* C(n+k-1,k+1) = C(n+k-1,k) * (n+k) / (k+1) */
        term = (term * (seq_t)(n + k)) / ((seq_t)(k + 1));
        sum_sq += term * term;
    }

    /* Final value: S1^4 * S2^3 */
    seq_t pow_S1 = pow_seq(S1, 4);
    seq_t pow_S2 = pow_seq(sum_sq, 3);
    return pow_S1 * pow_S2;
}
