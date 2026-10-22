/*
 * A385620  E.g.f. A(x) satisfies A(x) = exp( x*(A(2*x) + A(3*x)) ).
 * https://oeis.org/A385620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n) = Σ_{k=0}^{n-1}
 *            (k+1)*(2^k+3^k)*binomial(n-1,k)*a(k)*a(n-1-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
#define ALLOC_ARRAY(T, n) new T[n]
#define FREE_ARRAY(p)     delete[] (p)
#else
#define ALLOC_ARRAY(T, n) ((T*)malloc((size_t)(n)*sizeof(T)))
#define FREE_ARRAY(p)     free((void*)(p))
#endif

seq_t a(idx_t n)
{
    /* Allocate temporary arrays for powers of 2 and 3, and the sequence itself */
    seq_t *pow2 = ALLOC_ARRAY(seq_t, n + 1);
    seq_t *pow3 = ALLOC_ARRAY(seq_t, n + 1);
    seq_t *seq = ALLOC_ARRAY(seq_t, n + 1);

    /* Fundamental powers: 2^0=3^0=1 */
    pow2[0] = 1;
    pow3[0] = 1;
    seq[0] = 1;                      /* a(0) */

    /* Pre‑compute powers of 2 and 3 up to n */
    for (idx_t i = 1; i <= n; ++i) {
        pow2[i] = pow2[i - 1] * (seq_t)2;
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }

    /* Compute the sequence using the recurrence */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t binom = 1;                   /* C(m-1,0) */

        for (idx_t k = 0; k < m; ++k) {
            /* term = a(k) * a(m-1-k) * binom * (k+1) * (2^k+3^k) */
            seq_t term = seq[k];
            term *= seq[m - 1 - k];
            term *= binom;
            term *= (seq_t)(k + 1);
            term *= (pow2[k] + pow3[k]);

            sum += term;

            /* Update binomial for next k: C(m-1, k+1) = C(m-1, k) * (m-1-k)/(k+1) */
            if (k + 1 < m) {
                binom = binom * (seq_t)(m - 1 - k) / (seq_t)(k + 1);
            }
        }
        seq[m] = sum;
    }

    seq_t result = seq[n];

    /* Free allocated memory */
    FREE_ARRAY(pow2);
    FREE_ARRAY(pow3);
    FREE_ARRAY(seq);

    return result;
}
