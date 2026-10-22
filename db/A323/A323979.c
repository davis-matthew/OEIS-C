/*
 * A323979  Expansion of determinant of 120 X 120 matrix M_{u,v} =
 *          q^(maj(u*v^-1)) where u, v in S_5 and maj = MacMahon's major index
 *          (cf. A008302).
 * https://oeis.org/A323979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of q^n in (1-q^2)^60 (1-q^3)^80 (1-q^4)^90
 *            (1-q^5)^96
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 39, i128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Lazy initialization of the coefficient array */
#define MAX_DEGREE 1200

static seq_t coeff[MAX_DEGREE + 1];
static bool initialized = false;

/* Multiplicative binomial coefficient (C(n,k)) */
static seq_t binom_idx(idx_t n, idx_t k) {
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i) / (seq_t)i;
    }
    return r;
}

/* Build the coefficient array once */
static void init_coeffs(void) {
    /* Start with polynomial 1 */
    for (idx_t i = 0; i <= MAX_DEGREE; ++i) coeff[i] = 0;
    coeff[0] = 1;

    /* Factors: (1 - q^k)^a with k=2,3,4,5 and corresponding a */
    const idx_t ks[4] = {2, 3, 4, 5};
    const idx_t as[4] = {60, 80, 90, 96};

    static seq_t tmp[MAX_DEGREE + 1];
    for (int f = 0; f < 4; ++f) {
        idx_t k = ks[f];
        idx_t a = as[f];

        /* Precompute the polynomial for this factor:
           P(x) = sum_{j=0}^{a} (-1)^j * C(a,j) * x^{k*j} */
        seq_t term_coeff[97]; /* a <= 96 */
        for (idx_t j = 0; j <= a; ++j) {
            seq_t bc = binom_idx(a, j);
            if (j & 1) term_coeff[j] = -bc;
            else       term_coeff[j] = bc;
        }

        /* Convolution with current coefficient array */
        for (idx_t i = 0; i <= MAX_DEGREE; ++i) tmp[i] = 0;
        for (idx_t i = 0; i <= MAX_DEGREE; ++i) {
            if (coeff[i] == 0) continue;
            for (idx_t j = 0; j <= a; ++j) {
                idx_t idx = i + k * j;
                if (idx > MAX_DEGREE) break;
                tmp[idx] += coeff[i] * term_coeff[j];
            }
        }
        /* Copy result back */
        for (idx_t i = 0; i <= MAX_DEGREE; ++i) coeff[i] = tmp[i];
    }

    initialized = true;
}

/* Sequence function */
seq_t a(idx_t n) {
    if (!initialized) init_coeffs();
    if (n > MAX_DEGREE) return (seq_t)0;
    return coeff[(idx_t)n];
}
