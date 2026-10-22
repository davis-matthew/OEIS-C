/*
 * A377893  E.g.f. A(x) satisfies A(x) = (1 + x*A(x)^3) * exp(x * A(x)^3).
 * https://oeis.org/A377893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A377893
   Formula: a(n) = n! * Σ_{k=0..n} (3*n+1)^(k-1) * binomial(3*n+1, n-k) / k! */

#include <stdint.h>

/* seq_t and idx_t are provided by the build system. */

/* Compute the term a(n) of sequence A377893. */
seq_t a(idx_t n)
{
    /* Base value: N = 3*n + 1 */
    seq_t N = (seq_t)(3 * n + 1);

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute binomial(N, n)  multiplicatively.
       C(N,n) = Π_{i=1..n} (N-n+i)/i
       Since N-n+i = 2*n + i + 1, we can use that directly. */
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i) {
        binom = binom * (seq_t)(2 * n + i + 1) / (seq_t)i;
    }

    /* Sum accumulator. */
    seq_t sum = 0;

    /* Term for k = 0:  n! * C(N,n) / N */
    sum += fact * binom / N;

    /* If n == 0, we are done. */
    if (n == 0) {
        return sum;
    }

    /* Prepare for the loop over k = 1 .. n */
    seq_t fac_rem = fact;   /* holds n! / (k-1)! for current k */
    seq_t powN   = 1;       /* holds N^(k-1), starts with N^0 at k=1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update factorial part: divide by k to get n!/k! */
        fac_rem /= (seq_t)k;

        /* Update the binomial: from C(N, n-(k-1)) to C(N, n-k) */
        seq_t numerator   = (seq_t)(n - k + 1);
        seq_t denominator = (seq_t)(2 * n + k + 1);   /* = N - n + k */
        binom = binom * numerator / denominator;

        /* Compute term_k: fac_rem * powN * binom  (all integer arithmetic) */
        seq_t term = fac_rem * powN * binom;
        sum += term;

        /* Prepare powN for next iteration: multiply by N */
        powN *= N;
    }

    return sum;
}
