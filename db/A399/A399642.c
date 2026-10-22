/*
 * A399642  Expansion of g^6/(4-3*g)^4, where g = 1+x*g^4 is the g.f. of
 *          A002293.
 * https://oeis.org/A399642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} 3^k · C(k+2,2) · C(4·n+5, n−k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = Sum_{k=0..n} 3^k * binomial(k+2,2) * binomial(4*n+5, n-k)   */
/*  Implements the closed‑form summation directly; O(n) arithmetic operations per call. */

/*  For clarity:
 *      binomial(k+2,2) = ((k+2)*(k+1))/2
 *      binomial(4n+5, n-k) is updated successively by the identity
 *      C(N, j-1) = C(N, j) * j / (N-j+1) with N = 4n+5.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* N = 4*n + 5  and  C(N, n) used as the first binomial term */
    idx_t N = 4*n + 5;
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i) {
        binom = binom * (N - (i-1)) / i;   /* binom = C(N, i) */
    }

    seq_t pow3 = 1;          /* 3^k for current k */
    idx_t j    = n;          /* current j = n - k in binomial term */

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(k+2,2) = (k+2)*(k+1)/2  (exact integer division) */
        seq_t comb_k = ((seq_t)(k+2) * (k+1)) / 2;

        /* term = 3^k * C(k+2,2) * C(4n+5, n-k) */
        seq_t term   = pow3 * comb_k;
        term        *= binom;
        result      += term;

        /* prepare for next iteration */
        pow3 *= 3;
        if (j > 0) {
            binom = binom * j / (N - j + 1);  /* binom = C(N, j-1) */
            --j;
        }
    }

    return result;
}
