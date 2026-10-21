/*
 * A289344  Coefficients in expansion of E_2^(1/2)/Product_{k>=1} (1-q^k).
 * https://oeis.org/A289344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0..n} b(k)·p(n‑k) where • p(0)=1 and p(m) = Σ_{k≥1}
 *            (-1)^{k+1}\,[p(m‑g(k)) + p(m‑g'(k))] (pentagonal number theorem,
 *            g(k)=k(3k‑1)/2, g'(k)=k(3k+1)/2) • b(0)=1 and for m≥1, 2·b(m) +
 *            Σ_{i=1}^{m-1} b(i)·b(m‑i) = –24·σ₁(m) (σ₁(m) = sum of divisors
 *            of m) The implementation computes σ₁, par...
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 14, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    size_t N = (size_t)(n + 1);

    /* allocate and zero all working arrays */
    seq_t *p      = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *sigma  = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *b      = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *a_arr  = (seq_t *)calloc(N, sizeof(seq_t));

    if (!p || !sigma || !b || !a_arr) {
        /* allocation failure – free any that were allocated */
        free(p); free(sigma); free(b); free(a_arr);
        return (seq_t)0;
    }

    /* 1. compute σ₁(k) for 1 ≤ k ≤ n */
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t m = k; m <= n; m += k)
            sigma[m] += (seq_t)k;
    }

    /* 2. partition numbers via pentagonal theorem */
    p[0] = (seq_t)1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            seq_t sign = (k & 1) ? (seq_t)1 : -(seq_t)1;
            sum += sign * p[m - g1];
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > m) continue;
            sum += sign * p[m - g2];
        }
        p[m] = sum;
    }

    /* 3. compute b(k) */
    b[0] = (seq_t)1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t prod_sum = 0;
        for (idx_t i = 1; i < m; ++i)
            prod_sum += b[i] * b[m - i];
        seq_t numer = -(seq_t)24 * sigma[m] - prod_sum;
        b[m] = numer / 2;   /* division is exact */
    }

    /* 4. convolution to obtain a(n) */
    a_arr[0] = (seq_t)1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t val = 0;
        for (idx_t k = 0; k <= m; ++k)
            val += b[k] * p[m - k];
        a_arr[m] = val;
    }

    seq_t result = a_arr[n];

    /* free temporary storage */
    free(p);
    free(sigma);
    free(b);
    free(a_arr);

    return result;
}
