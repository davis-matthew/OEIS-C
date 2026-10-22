/*
 * A383323  Expansion of e.g.f. (1+x)*(exp(x)-1)*(exp(x)-x)*(exp(x)-x^2/2).
 * https://oeis.org/A383323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n-1)*(n+3) -
 *            2^(n-3)*(3*binomial(n,3)+6*binomial(n,2)+8*n+8) * +
 *            12*binomial(n,4)+6*binomial(n,3)+3*binomial(n,2)+n * with
 *            a(0)=0, a(1)=1, a(2)=5, a(3)=16, a(4)=59 (for n>=5 use the
 *            formula)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* handle the small indices explicitly */
    if (n <= 4) {
        static const seq_t small[5] = { 0, 1, 5, 16, 59 };
        return small[n];
    }

    /* compute combinatorial terms */
    seq_t cn3 = binomial(n, 3);   /* C(n,3) */
    seq_t cn2 = binomial(n, 2);   /* C(n,2) */
    seq_t cn4 = binomial(n, 4);   /* C(n,4) */

    /* powers of 3 and 2 */
    seq_t pow3 = pow_uint(3, n - 1);   /* 3^(n-1) */
    seq_t pow2 = pow_uint(2, n - 3);   /* 2^(n-3) */

    /* term1 = 3^(n-1)*(n+3) */
    seq_t term1 = pow3 * ((seq_t)n + (seq_t)3);

    /* term2_inner = 3*binom(n,3) + 6*binom(n,2) + 8*n + 8 */
    seq_t term2_inner = (seq_t)3 * cn3 + (seq_t)6 * cn2
                       + (seq_t)8 * n + (seq_t)8;

    /* term2 = 2^(n-3) * term2_inner */
    seq_t term2 = pow2 * term2_inner;

    /* remaining polynomial terms */
    seq_t term3 = (seq_t)12 * cn4;
    seq_t term4 = (seq_t)6  * cn3;
    seq_t term5 = (seq_t)3  * cn2;
    seq_t term6 = (seq_t)n;

    /* assemble result */
    seq_t result = term1 - term2 + term3 + term4 + term5 + term6;
    return result;
}
