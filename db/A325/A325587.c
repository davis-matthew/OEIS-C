/*
 * A325587  G.f.: Sum_{n>=0} (n+1)*(n+2)*(n+3)/3! * x^n * (1+x)^(n*(n+3)).
 * https://oeis.org/A325587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A325587
    Formula: a(n) = Σ_{k=0..n} (k+1)(k+2)(k+3)/6 * binom(k(k+3), n-k)
    where binom(N, K) is the usual binomial coefficient.  */

/*  Compute a single term.  A direct implementation runs in O(n²)
    time and requires only O(1) auxiliary space.  It works for any
    integer type seq_t (fixed‑width or arbitrary precision) that
    supports + – * / and comparison operators.  */

#include <stdint.h>      /* required by some build environments */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t result = 0;

    /* Sum over k = 0..n.  For each k add the contribution
       (k+1)(k+2)(k+3)/6 * binom(k(k+3), n-k) */
    for (idx_t k = 0; k <= n; ++k)
    {
        /* C(k) = (k+1)(k+2)(k+3)/6 */
        seq_t Ck = ((seq_t)(k + 1) * (seq_t)(k + 2) * (seq_t)(k + 3)) / 6;

        /* N = k*(k+3) is the upper index in the binomial */
        idx_t N = k * (k + 3);

        /* K = n - k is the lower index in the binomial */
        idx_t K = n - k;
        if (K > N)
            continue;                 /* binom is zero, skip */

        /* Compute binom(N, K) multiplicatively:
           binom( N, K ) = Π_{i=1..K} (N-K+i)/i
           This is exact at every step. */
        seq_t bin = 1;
        for (idx_t i = 1; i <= K; ++i)
        {
            bin = bin * (N - K + i);
            bin = bin / i;
        }

        result += Ck * bin;
    }

    return result;
}
