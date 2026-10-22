/*
 * A385607  Diagonal of the rational function 1/(1 - (v^2 + w^2 + x^2 + y^2 +
 *          z^2 + v*w*x*y*z)).
 * https://oeis.org/A385607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n+3*k)!/(k!^5*(n-2*k)!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Computes the nth term of OEIS sequence A385607.
 * The result is returned as type seq_t.   */
seq_t a(idx_t n)
{
    /* Sum of the terms up to k = floor(n/2). */
    seq_t sum = 0;
    /* Current term T_k (starting with k = 0). */
    seq_t term = 1;

    /* Add the k = 0 term (equals 1). */
    sum += term;

    idx_t maxk = n / 2;          /* floor(n/2) */

    for (idx_t k = 1; k <= maxk; ++k)
    {
        /* Components needed for the recurrence: */
        /*       (n+3k-2)*(n+3k-1)*(n+3k) * (n-2k+2)*(n-2k+1)  /  k^5   */
        seq_t f1 = (seq_t)(n + 3 * k - 2);
        seq_t f2 = (seq_t)(n + 3 * k - 1);
        seq_t f3 = (seq_t)(n + 3 * k);
        seq_t f4 = (seq_t)(n - 2 * k + 2);
        seq_t f5 = (seq_t)(n - 2 * k + 1);

        seq_t denom = 1;
        for (int i = 0; i < 5; ++i)
            denom *= (seq_t)k;   /* k^5 */

        /* Multiply by the numerator factors, cancelling with denom as we go. */
        seq_t f = f1;
        seq_t g = gcd(f, denom);
        if (g > (seq_t)1) { f /= g; denom /= g; }
        term *= f;
        g = gcd(term, denom);
        if (g > (seq_t)1) { term /= g; denom /= g; }

        f = f2;
        g = gcd(f, denom);
        if (g > (seq_t)1) { f /= g; denom /= g; }
        term *= f;
        g = gcd(term, denom);
        if (g > (seq_t)1) { term /= g; denom /= g; }

        f = f3;
        g = gcd(f, denom);
        if (g > (seq_t)1) { f /= g; denom /= g; }
        term *= f;
        g = gcd(term, denom);
        if (g > (seq_t)1) { term /= g; denom /= g; }

        f = f4;
        g = gcd(f, denom);
        if (g > (seq_t)1) { f /= g; denom /= g; }
        term *= f;
        g = gcd(term, denom);
        if (g > (seq_t)1) { term /= g; denom /= g; }

        f = f5;
        g = gcd(f, denom);
        if (g > (seq_t)1) { f /= g; denom /= g; }
        term *= f;
        g = gcd(term, denom);
        if (g > (seq_t)1) { term /= g; denom /= g; }

        /* denom should be 1 now; any remaining factor is cancelled. */
        /* Add the new term to the sum. */
        sum += term;
    }

    return sum;
}
