/*
 * A294978  Coefficients in expansion of (E_4/E_2^4)^(1/8).
 * https://oeis.org/A294978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in (E_4/E_2^4)^(1/8) where E_2(q) = 1
 *            - 24∑σ₁(m)q^m, E_4(q) = 1 + 240∑σ₃(m)q^m. Computation uses the
 *            identity a(0)=1, and for n≥1 a(n) = (1/(8n)) * Σ_{k=1}^{n} b(k)
 *            * a(n-k), where b(k) = ∑_{d|k} d c(d) and the exponents c(d) are
 *            obtained from the logarithm of f(q)=E_4/E...
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 4, i64 n <= 8, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>   /* for memset */

/* ------------------------------------------------------------------ */
/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* --- pre‑compute divisor sums ------------------------------------------------- */
    uint64_t *sigma1 = (uint64_t*)malloc((n + 1) * sizeof(uint64_t));
    uint64_t *sigma3 = (uint64_t*)malloc((n + 1) * sizeof(uint64_t));
    if (!sigma1 || !sigma3) exit(EXIT_FAILURE);
    memset(sigma1, 0, (n + 1) * sizeof(uint64_t));
    memset(sigma3, 0, (n + 1) * sizeof(uint64_t));

    for (idx_t i = 1; i <= n; ++i) {
        uint64_t ii = (uint64_t)i;
        uint64_t cube = ii * ii * ii;
        for (idx_t j = i; j <= n; j += i) {
            sigma1[j] += ii;
            sigma3[j] += cube;
        }
    }

    /* --- coefficients of E2 and E4 ------------------------------------------------ */
    seq_t *e2  = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *e4  = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!e2 || !e4) exit(EXIT_FAILURE);

    e2[0] = (seq_t)1;
    e4[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        e2[i] = (seq_t)(-24) * (seq_t)sigma1[i];
        e4[i] = (seq_t)240  * (seq_t)sigma3[i];
    }

    /* --- scaled log coefficients for E2 and E4 ------------------------------------ */
    seq_t *u2  = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *u4  = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!u2 || !u4) exit(EXIT_FAILURE);
    u2[0] = u4[0] = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        /* u2[k] = k*e2[k] - Σ_{i=1}^{k-1} u2[i]*e2[k-i] */
        seq_t sum = (seq_t)k * e2[k];
        for (idx_t i = 1; i < k; ++i) {
            sum -= u2[i] * e2[k - i];
        }
        u2[k] = sum;

        /* u4[k] = k*e4[k] - Σ_{i=1}^{k-1} u4[i]*e4[k-i] */
        sum = (seq_t)k * e4[k];
        for (idx_t i = 1; i < k; ++i) {
            sum -= u4[i] * e4[k - i];
        }
        u4[k] = sum;
    }

    /* --- compute b(k) --------------------------------------------------------- */
    seq_t *b = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!b) exit(EXIT_FAILURE);
    b[0] = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        b[k] = u4[k] - (seq_t)4 * u2[k];
    }

    /* --- compute a(n) ---------------------------------------------------------- */
    seq_t *a_arr = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!a_arr) exit(EXIT_FAILURE);
    a_arr[0] = (seq_t)1;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += b[k] * a_arr[m - k];
        }
        a_arr[m] = sum / ((seq_t)8 * (seq_t)m);
    }

    seq_t result = a_arr[n];

    /* free allocated memory */
    free(sigma1); free(sigma3);
    free(e2);    free(e4);
    free(u2);    free(u4);
    free(b);
    free(a_arr);

    return result;
}
