/*
 * A380727  E.g.f. A(x) satisfies A(x) = exp(x * A(x)^3 / (1 - x*A(x)^3)) / (1
 *          - x*A(x)^3).
 * https://oeis.org/A380727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (3*n+1)^(k-1) * binomial(4*n,n-k)/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, factorial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A380727 using a recurrence that keeps all intermediate values exact */

#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/gcd.h"

/* Return the n‑th term of OEIS A380727 (offset 0). */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* Precompute constants */
    const seq_t fact_n   = factorial(n);          /* n! */
    const seq_t const3n1 = (seq_t)(3 * n + 1);    /* 3n+1 */

    /* First term (k = 0) : n! * binom(4n,n) / (3n+1) */
    seq_t term = binomial(4 * n, n * 1ULL);       /* binom(4n,n) */
    term *= fact_n;                               /* * n!         */
    term /= const3n1;                             /* / (3n+1)     */
    seq_t sum  = term;                            /* accumulate */

    /* Recurrence for k = 1 .. n:
       term_k = term_{k-1} * (3n+1) * (n-k+1) / (k * (3n + k))
    */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Factors for the recurrence */
        seq_t a1 = (seq_t)(n - k + 1);           /* (n‑k+1)          */
        seq_t a2 = const3n1;                    /* (3n+1)           */
        seq_t denom = (seq_t)k * ((seq_t)3 * n + k); /* k * (3n + k) */

        /* Cancel common factors between a1, a2 and denom */
        seq_t g = gcd(a1, denom);
        a1  /= g;
        denom /= g;

        g = gcd(a2, denom);
        a2  /= g;
        denom /= g;

        /* Also cancel factors of term with denom */
        g = gcd(term, denom);
        term /= g;
        denom /= g;

        /* Multiply remaining factors and finish division */
        term *= a1;
        term *= a2;
        term /= denom;                           /* exact division */

        sum += term;                             /* accumulate */
    }

    return sum;
}
