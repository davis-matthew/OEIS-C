/*
 * A307762  L.g.f.: log(Product_{k>=1} (1 + x^k/(1 + x))) = Sum_{k>=1}
 *          a(k)*x^k/k.
 * https://oeis.org/A307762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n*f_n - sum_{k=1}^{n-1} f_k*a(n-k) where f_n is the
 *            coefficient of x^n in P(x)=∏_{j≥1}(1 + x^j/(1+x))
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It works for any seq_t/idx_t types defined in the build. */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;               /* undefined for n=0, return 0 */
    }

    size_t N = (size_t)n;        /* assume n fits in size_t for memory allocation */
    size_t sz = N + 1;

    /* Allocate arrays for the product coefficients f, alternating prefix sums, and a. */
    seq_t *p  = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *alt = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *a_arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!p || !alt || !a_arr) {
        /* on allocation failure, return 0 */
        free(p); free(alt); free(a_arr);
        return 0;
    }

    /* Initialize the product: P(x) = 1. */
    p[0] = 1;
    for (size_t i = 1; i <= N; ++i) p[i] = 0;

    /* Compute the product coefficients f_n. */
    for (size_t k = 1; k <= N; ++k) {
        /* Compute alternating prefix sums of current p. */
        alt[0] = p[0];
        for (size_t i = 1; i <= N; ++i) {
            seq_t sign = (i % 2 == 0) ? (seq_t)1 : (seq_t)-1;
            alt[i] = alt[i - 1] + sign * p[i];
        }

        /* Update p in place for the factor (1 + x^k/(1+x)). */
        for (size_t e = 0; e <= N; ++e) {
            if (e < k) continue;          /* coefficient unchanged */
            seq_t old = p[e];
            seq_t sign = (((e - k) % 2) == 0) ? (seq_t)1 : (seq_t)-1;
            p[e] = old + sign * alt[e - k];
        }
    }

    /* Compute a_n using the recurrence derived from the logarithm identity. */
    a_arr[0] = 0;
    for (size_t n_i = 1; n_i <= N; ++n_i) {
        seq_t temp = (seq_t)n_i * p[n_i];
        seq_t sum = 0;
        for (size_t k = 1; k < n_i; ++k) {
            sum += p[k] * a_arr[n_i - k];
        }
        a_arr[n_i] = temp - sum;
    }

    seq_t result = a_arr[N];
    free(p);
    free(alt);
    free(a_arr);
    return result;
}
