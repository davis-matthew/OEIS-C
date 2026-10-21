/*
 * A246618  Number of endofunctions on [2n] whose cycle lengths are multiples
 *          of n.
 * https://oeis.org/A246618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n-1)! + C(2n,n)*(n-1)!^2/2 +
 *            C(2n,n)*(n-1)!*n*(2n)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, factorial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(0)=1,  a(1)=4,  a(2)=57,  a(3)=4480, … (OEIS A246618) */

#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b); */
#include "../utils/factorial.h"   /* seq_t factorial(idx_t n);      */
#include "../utils/pow_uint.h"    /* seq_t pow_uint(uint64_t base, idx_t exp); */

static seq_t pow_uint_u64(seq_t base, idx_t exp)
{
    /* wrapper around utils pow_uint that accepts seq_t base */
    return pow_uint((uint64_t)base, exp);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t two_n = n << 1;                 /* 2*n */

    /* term1 = (2n-1)! */
    seq_t term1 = factorial(two_n - 1);

    /* binomial coefficient C(2n,n) */
    seq_t bin = binomial(two_n, n);

    /* (n-1)! */
    seq_t fact_n_minus1 = factorial(n - 1);

    /* term2 = C(2n,n) * ((n-1)!)^2 / 2 */
    seq_t term2 = bin * fact_n_minus1 * fact_n_minus1;
    term2 /= (seq_t)2;

    /* term3 = C(2n,n) * (n-1)! * n * (2n)^(n-1) */
    seq_t pow_term = pow_uint_u64((seq_t)two_n, n - 1);
    seq_t term3 = bin * fact_n_minus1 * (seq_t)n * pow_term;

    return term1 + term2 + term3;
}
