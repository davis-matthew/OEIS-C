/*
 * A377529  Expansion of e.g.f. 1/(1 - x * exp(x))^2.
 * https://oeis.org/A377529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1) * k^(n-k)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation using the e.g.f. 1/(1 - x*exp(x))^2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static arrays: b[n] for coefficients of f(x)=1/(1-xexp(x)),
       a_arr[n] for the required sequence terms */
    static seq_t *b = NULL;
    static seq_t *a_arr = NULL;
    static idx_t computed = 0;    /* highest n already computed */

    /* initialise storage */
    if (!b) {
        b = (seq_t *)malloc(sizeof(seq_t));
        a_arr = (seq_t *)malloc(sizeof(seq_t));
        if (!b || !a_arr) return (seq_t)0;   /* out of memory; caller may ignore */
        b[0] = (seq_t)1;      /* f(0) = 1 */
        a_arr[0] = (seq_t)1;  /* a(0) = 1 */
        computed = 0;
    }

    /* extend precomputation up to requested n */
    if (n > computed) {
        idx_t new_cap = n + 1;
        b = (seq_t *)realloc(b, new_cap * sizeof(seq_t));
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (!b || !a_arr) return (seq_t)0; /* allocation failure */

        for (idx_t m = computed + 1; m <= n; ++m) {
            /* compute b[m] using recurrence:   b_m = m * Σ_{i=0}^{m-1} C(m-1,i) b_i   */
            seq_t sum_b = (seq_t)0;
            seq_t binom = (seq_t)1;    /* C(m-1,0) */
            for (idx_t i = 0; i < m; ++i) {
                sum_b += binom * b[i];
                /* update binom to C(m-1,i+1) */
                binom = (binom * (seq_t)(m - 1 - i)) / ((seq_t)(i + 1));
            }
            b[m] = (seq_t)m * sum_b;

            /* compute a[m] from convolution:   a_m = Σ_{i=0}^{m} C(m,i) b_i b_{m-i}   */
            seq_t val_a = (seq_t)0;
            binom = (seq_t)1;    /* C(m,0) */
            for (idx_t i = 0; i <= m; ++i) {
                val_a += binom * b[i] * b[m - i];
                /* update binom to C(m,i+1) */
                binom = (binom * (seq_t)(m - i)) / ((seq_t)(i + 1));
            }
            a_arr[m] = val_a;
        }
        computed = n;
    }

    return a_arr[n];
}
