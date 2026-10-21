/*
 * A292998  Number of sequences of balls colored with at most n colors such
 *          that exactly three balls are the same color as some other ball in
 *          the sequence.
 * https://oeis.org/A292998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=3..n+2} binomial(k,3)/(n+2-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* The external build defines seq_t, idx_t and the standard arithmetic
   operators on seq_t.  No other utilities are required. */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1; a(1) is the first term. */
    if (n == 0) {
        return 0;             /* undefined, but safe for caller */
    }

    /* m = n + 2 is used in the formulation below.   */
    idx_t m = n + 2;

    /* We need factorials up to m (i.e., up to n+2). */
    size_t sz = (size_t)(m + 1);     /* indices 0 .. m inclusive */
    seq_t *fac = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!fac) {
        return 0;                    /* allocation failure */
    }

    fac[0] = (seq_t)1;
    for (idx_t i = 1; i <= m; ++i) {
        fac[(size_t)i] = fac[(size_t)(i - 1)] * (seq_t)i;
    }

    seq_t sum = (seq_t)0;

    /* The summation variable t runs from 0 to n-1, corresponding to
       k = n + 2 - t.  Every term is an integer:
          term = binomial(n+2-t, 3) * (n! / t!)
       where n! / t! = product of (t+1 .. n) = fac[n] / fac[t].   */
    for (idx_t t = 0; t < n; ++t) {
        idx_t A = n + 2 - t;                 /* n+2-t, the upper argument of binomial */

        /* binomial(A,3) = A*(A-1)*(A-2)/6 = fac[A]/fac[A-3]/6.
           fac[A-3] == fac[n-1-t] because A = n+2-t.            */
        seq_t bin = fac[(size_t)A] / fac[(size_t)(n - 1 - t)];
        bin /= 6;                                 /* exact division */

        /* product of (t+1 .. n) = fac[n] / fac[t] */
        seq_t prod = fac[(size_t)n] / fac[(size_t)t];

        seq_t term = bin * prod;
        sum += term;
    }

    free(fac);
    return sum;
}
