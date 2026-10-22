/*
 * A365798  G.f. satisfies A(x) = 1 + x*A(x)*(1 + x^5*A(x)^4).
 * https://oeis.org/A365798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/6⌋} binomial(n-5k,k) * binomial(n-k+1,n-5k)
 *            / (n-k+1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the sum directly using multiplicative
   binomial coefficients and reduces intermediate values to avoid
   overflow.  It is valid for all seq_t and idx_t types supplied by the build. */

#include <stdbool.h> /* for bool (if needed) */

/* ------------------------------------------------------------------ */
/* local helper: Euclidean gcd for seq_t                              */
static seq_t _gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* local helper: binomial coefficient computed multiplicatively      */
/* (returns C(n,k)).  Uses k = min(k, n-k) for efficiency.           */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k == 0 || k == n)
        return (seq_t)1;

    k = (k > n - k) ? n - k : k;      /* choose smaller k */
    seq_t result = (seq_t)1;

    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by (n - k + i) and divide by i; this keeps intermediate
           results exact as integer division is exact at each step.          */
        result *= (seq_t)(n - k + i);
        result /= (seq_t)i;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;

    idx_t maxk = n / 6;          /* floor(n/6) */
    for (idx_t k = 0; k <= maxk; ++k) {
        /* compute terms of the summand */
        idx_t n1   = n - 5 * k;   /* n-5k */
        idx_t den  = n - k + 1;   /* n-k+1 */

        seq_t bin1 = _binom(n1, k);           /* binomial(n-5k, k)   */
        seq_t bin2 = _binom(den, n1);          /* binomial(n-k+1, n-5k) */

        /* reduce bin1 with the denominator first */
        seq_t g1 = _gcd(bin1, (seq_t)den);
        bin1 /= g1;
        den /= (idx_t)g1;

        /* reduce maybe remaining denominator with bin2 */
        seq_t g2 = _gcd(bin2, (seq_t)den);
        bin2 /= g2;
        den /= (idx_t)g2;

        /* now compute product and final division */
        seq_t term = (bin1 * bin2) / (seq_t)den;
        sum += term;
    }
    return sum;
}
