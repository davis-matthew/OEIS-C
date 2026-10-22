/*
 * A381818  Expansion of ( (1/x) * Series_Reversion( x * ((1-x) / C(x))^2 )
 *          )^(1/2), where C(x) is the g.f. of A000108.
 * https://oeis.org/A381818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+2*k+1,k) *
 *            binomial(3*n-k,n-k)/(2*n+2*k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence value for index n using only integer arithmetic.
   The computation is performed in two stages:
     1. Compute the term for k = 0:  T0 = binom(3*n, n) / (2*n+1).
        The binomial coefficient is evaluated multiplicatively so that
        intermediate values never overflow a 128‑bit integer.
     2. For k = 1 .. n use the recurrence
            T_k = T_{k-1} * ( (2n+2k+1)(2n+2k)(n-k+1)(2n+2k-1) )
                                   / ( k(2n+k+1)(3n-k+1)(2n+2k+1) )
        which follows directly from the ratio of consecutive terms.
        All intermediate products are performed in unsigned __int128
        and the division is exact by construction. */

#include <stdint.h>   /* for uint64_t when seq_t is 64‑bit */

seq_t a(idx_t n)
{
    /* Handle the trivial case first. */
    if (n == 0)
        return (seq_t)1;

    /* ----- 1. Compute T0 = binom(3n, n) / (2n+1) ---------------------- */
    seq_t term = 1;          /* will hold T_k */
    seq_t prod = 1;          /* product for binom(3n, n) */

    /* C(3n, n) = prod_{i=1..n} (2n + i) / i */
    for (idx_t i = 1; i <= n; ++i) {
        unsigned __int128 tmp = (unsigned __int128)prod * (2*n + i);
        prod = (seq_t)(tmp / i);
    }

    /* Divide by (2n+1) to obtain T0. */
    unsigned __int128 tmp = (unsigned __int128)prod;
    term = (seq_t)(tmp / (2*n + 1));

    seq_t sum = term;        /* first term contributes to the sum */

    /* ----- 2. Iterate over k = 1 .. n to accumulate remaining terms --- */
    for (idx_t k = 1; k <= n; ++k) {
        /* Common subexpressions in the ratio formula */
        idx_t a1 = 2*n + 2*k + 1;          /* 2n + 2k + 1 */
        idx_t a2 = 2*n + 2*k;              /* 2n + 2k     */
        idx_t b1 = n - k + 1;              /* n - k + 1   */
        idx_t c1 = 2*n + 2*(k-1) + 1;      /* 2n + 2(k-1)+1 = 2n + 2k - 1 */

        idx_t d1 = k;                      /* k           */
        idx_t d2 = 2*n + k + 1;            /* 2n + k + 1  */
        idx_t d3 = 3*n - k + 1;            /* 3n - k + 1  */
        idx_t d4 = 2*n + 2*k + 1;          /* 2n + 2k + 1 */

        unsigned __int128 num = (unsigned __int128)a1 * a2;
        num *= b1;
        num *= c1;

        unsigned __int128 den = (unsigned __int128)d1;
        den *= d2;
        den *= d3;
        den *= d4;

        /* Compute next term: term = term * num / den */
        unsigned __int128 prod_term = (unsigned __int128)term * num;
        term = (seq_t)(prod_term / den);

        sum += term;
    }

    return sum;
}
