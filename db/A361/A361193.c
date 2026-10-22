/*
 * A361193  E.g.f. satisfies A(x) = exp( -2*x*A(x) ) / (1-x).
 * https://oeis.org/A361193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies (1‑x)A(x)=exp(-2xA(x))
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived:  a(0)=1,  t(0)=1
 * For n>=1,
 *   t(n) = -2 * ∑_{k=1}^{n} k * a(k-1) * t(n-k) * C(n-1, k-1)
 *   a(n) = n*a(n-1) + t(n)
 *   (Here t(n)=n!*e_n and a(n) are the sequence terms.)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    /* Allocate space for a[0..n] and t[0..n] */
    seq_t *a = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *t = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!a || !t) {         /* Allocation failure: return 0 as sentinel */
        return (seq_t)0;
    }

    a[0] = (seq_t)1;   /* a(0)=1 */
    t[0] = (seq_t)1;   /* n!*e_n for n=0 => 0!*1 = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;         /* C(i-1,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* term = -2 * k * a[k-1] * t[i-k] * binom */
            seq_t term = a[k-1];
            term *= t[i-k];
            term *= (seq_t)k;
            term *= (seq_t)-2;
            term *= binom;
            sum += term;

            if (k != i) {
                /* Update binom to C(i-1, k) from C(i-1, k-1):
                 * binom = binom * (i - k) / k
                 */
                binom *= (seq_t)(i - k);
                binom /= (seq_t)k;
            }
        }

        t[i] = sum;
        a[i] = (seq_t)i * a[i-1] + t[i];
    }

    seq_t result = a[n];
    free(a);
    free(t);
    return result;
}
