/*
 * A367486  Expansion of e.g.f. 1/(3 - 2*exp(x))^x.
 * https://oeis.org/A367486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
    OEIS Formula:
    a(0) = 1
    a(n) = Sum_{k=1..n} A367489(k) * binomial(n-1,k-1) * a(n-k)

    A367489(k) = k * d_{k-1}  with  d_0 = 0
    d_1 = 2
    for n >= 2:
        d_n = 2 + 2 * Sum_{k=1}^{n-1} [ k * d_k * binomial(n-1,k) / (n-k) ]

    This is derived from the expansion of the e.g.f.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special handling for n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate arrays for d[0..n] and a[0..n] */
    seq_t *d = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *a_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!d || !a_arr) {
        /* If allocation fails, return 0 (should not happen in tests) */
        if (d) free(d);
        if (a_arr) free(a_arr);
        return (seq_t)0;
    }

    /* Initialize arrays */
    d[0] = (seq_t)0;          /* B_1 = 0 */
    d[1] = (seq_t)2;          /* d_1 = 2 */
    a_arr[0] = (seq_t)1;      /* a(0) = 1 */
    a_arr[1] = (seq_t)0;      /* a(1) = 0 */

    for (idx_t current = 2; current <= n; ++current) {
        /* Compute d_current */
        seq_t sum_d = (seq_t)0;
        seq_t binom_d = current - 1;  /* binomial(current-1,1) */

        for (idx_t k = 1; k <= current - 1; ++k) {
            seq_t term = d[k];           /* d_k */
            term *= (seq_t)k;            /* k * d_k */
            term *= binom_d;             /* * binomial(current-1,k) */
            term /= (current - k);       /* / (current - k) */
            sum_d += term;

            if (k < current - 1) {
                /* Update binom_d to binomial(current-1, k+1) */
                binom_d = binom_d * ((current - 1) - k) / (k + 1);
            }
        }
        d[current] = (seq_t)2 + ((seq_t)2 * sum_d);

        /* Compute a_current */
        seq_t sum_a = (seq_t)0;
        seq_t binom_a = (seq_t)1; /* binomial(current-1,0) */

        for (idx_t k = 1; k <= current; ++k) {
            seq_t Bk = (seq_t)k * d[k - 1];   /* A367489(k) */
            seq_t term_a = Bk;
            term_a *= binom_a;
            term_a *= a_arr[current - k];
            sum_a += term_a;

            if (k < current) {
                /* Update binom_a to binomial(current-1, k) */
                binom_a = binom_a * (current - k) / k;
            }
        }
        a_arr[current] = sum_a;
    }

    seq_t result = a_arr[n];

    /* Free allocated memory */
    free(a_arr);
    free(d);

    return result;
}
