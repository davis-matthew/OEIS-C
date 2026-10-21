/*
 * A259203  E.g.f.: Sum_{n>=0} x^n * (1 + x^n)^n * exp(3*x^(n+1)) / n!.
 * https://oeis.org/A259203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of xⁿ in A(x)=∑_{m≥0} xᵐ (1+xᵐ)ᵐ exp(3
 *            x^{m+1})/m! (e.g.f.). Computed by explicit summation over the
 *            indices that satisfy n = m(j+1)+t(m+1), 0≤j≤m, t≥0. The value is
 *            a(n)=∑_{m=1..n}∑_{t} (n!/(m!t!))·C(m,j)·3ᵗ + 3ⁿ (m=0 case)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static size_t fac_cap = 0;
static seq_t *fac = NULL;   /* factorials 0! … */

static size_t pow3_cap = 0;
static seq_t *pow3 = NULL;  /* powers 3⁰ … */

/* ensure factorials up to n are computed */
static void ensure_factorial(idx_t n) {
    if (fac_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_fac = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_fac) exit(1); /* out of memory */
        for (size_t i = 0; i < fac_cap; ++i) new_fac[i] = fac[i];
        for (size_t i = fac_cap; i < new_cap; ++i) {
            if (i == 0) new_fac[i] = 1;
            else new_fac[i] = new_fac[i-1] * (seq_t)((idx_t)i);
        }
        free(fac);
        fac = new_fac;
        fac_cap = new_cap;
    }
}

/* ensure powers of 3 up to n are computed */
static void ensure_pow3(idx_t n) {
    if (pow3_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_pow3 = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_pow3) exit(1); /* out of memory */
        for (size_t i = 0; i < pow3_cap; ++i) new_pow3[i] = pow3[i];
        for (size_t i = pow3_cap; i < new_cap; ++i) {
            if (i == 0) new_pow3[i] = 1;
            else new_pow3[i] = new_pow3[i-1] * (seq_t)3;
        }
        free(pow3);
        pow3 = new_pow3;
        pow3_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    ensure_factorial(n);
    ensure_pow3(n);

    seq_t result = 0;

    /* m = 0 term : 3ⁿ */
    result += pow3[n];

    for (idx_t m = 1; m <= n; ++m) {
        idx_t max_t = n / (m + 1);
        for (idx_t t = 0; t <= max_t; ++t) {
            idx_t rem = n - t * (m + 1);
            if (rem < m) continue;                /* j+1 ≥ 1 ⇒ rem ≥ m */
            if (rem % m != 0) continue;           /* j+1 integer */
            idx_t q = rem / m;                    /* q = j+1 */
            if (q > m + 1) continue;              /* j ≤ m */
            idx_t j = q - 1;                      /* 0 ≤ j ≤ m */

            /* binomial coefficient C(m, j) */
            seq_t binom_m_j = fac[m] / (fac[j] * fac[m - j]);

            /* ratio = n! / (m! t!) */
            seq_t ratio = fac[n];
            ratio /= fac[m];
            ratio /= fac[t];

            seq_t term = ratio * binom_m_j;
            term *= pow3[t];

            result += term;
        }
    }
    return result;
}
