/*
 * A262005  L.g.f.: log( Sum_{n>=0} x^n/n! * Product_{k=1..n} (k^5 + 1) ).
 * https://oeis.org/A262005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * f_n − Σ_{k=1}^{n−1} a(k) * f_{n−k}, where f_m =
 *            Π_{i=1}^m (i^5 + 1) / m! (with f_0 = 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The values a(n) are the coefficients of the ordinary power series
    L(x) = log( Σ_{m≥0} (x^m/m!) * Π_{i=1}^m (i^5 + 1) ).  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static idx_t computed = 0;        /* maximum n that has been computed */
    static seq_t *f_arr   = NULL;    /* f_m values, m = 0..computed */
    static seq_t *a_arr   = NULL;    /* a_m values, m = 0..computed */

    if (n == 0) return (seq_t)0;      /* undefined for n < 1, return 0 */

    if (n > computed) {
        /* Grow cache to hold values up to n */
        idx_t new_cap = n + 1;        /* indices 0 .. n */
        f_arr = (seq_t *)realloc(f_arr, new_cap * sizeof(seq_t));
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));

        /* initialise base cases if first call */
        if (computed == 0) {
            f_arr[0] = 1;  /* ∏ empty = 1, 0! = 1 */
            a_arr[0] = 0;  /* a(0) is not defined, use 0 as placeholder */
        }

        /* Compute f and a for all m = computed+1 .. n */
        for (idx_t m = computed + 1; m <= n; ++m) {
            /* f_m = f_{m-1} * (m^5 + 1) / m */
            seq_t m_pow5 = seq_pow((seq_t)m, 5);
            seq_t factor = m_pow5 + 1;
            seq_t prod = f_arr[m - 1] * factor;
            f_arr[m] = prod / (seq_t)m;

            /* a_m = m * f_m − Σ_{k=1}^{m−1} a_k * f_{m−k} */
            seq_t sum = 0;
            for (idx_t k = 1; k < m; ++k) {
                sum += a_arr[k] * f_arr[m - k];
            }
            a_arr[m] = ((seq_t)m * f_arr[m]) - sum;
        }

        computed = n;
    }

    return a_arr[n];
}
