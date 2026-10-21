/*
 * A274257  Number of factor-free Dyck words with slope 4/3 and length 7n.
 * https://oeis.org/A274257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the polynomial equation 0 = x*f^6 +
 *            x*(x-1)*f^5 - x^2*(x+1)*f^4 - x*(x-3)*(x+1)^2*f^3 +
 *            x*(x+1)^3*f^2 - (x+1)^4*f + (x+1)^5, where f(x) = Σ a(n)x^n and
 *            a(0)=1. The recurrence derived from this equation computes a(m)
 *            as a positive difference T_pos‑T_neg, where each T_pos and
 *            T_n...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_vals = NULL;          /* a[0..max_n]          */
    static seq_t *pow_f[7] = { NULL };   /* pow_f[k][0..max_n], k=1..6 */
    static idx_t max_n = (idx_t)-1;       /* lazily initialized */

    /* Lazy initialization */
    if (max_n == (idx_t)-1) {
        a_vals = (seq_t *)malloc(sizeof(seq_t));
        a_vals[0] = (seq_t)1;

        for (int k = 1; k <= 6; ++k) {
            pow_f[k] = (seq_t *)malloc(sizeof(seq_t));
            pow_f[k][0] = (seq_t)1; /* f^k(0) = 1 for every k≥1 */
        }
        max_n = 0;
    }

    /* Return cached result if available */
    if (n <= max_n)
        return a_vals[n];

    /* Extend storage to accommodate up to index n. */
    size_t new_cap = (size_t)(n + 1);
    a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    for (int k = 1; k <= 6; ++k)
        pow_f[k] = (seq_t *)realloc(pow_f[k], new_cap * sizeof(seq_t));

    /* Binomial coefficients for (x+1)^5. */
    const seq_t bin5[6] = { (seq_t)1, (seq_t)5, (seq_t)10,
                            (seq_t)10, (seq_t)5, (seq_t)1 };

    /* Compute all missing terms up to n. */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        seq_t T_pos = (seq_t)0;
        seq_t T_neg = (seq_t)0;

        /* 1. + f^6_{m-1} */
        if (m >= 1)   T_pos += pow_f[6][m - 1];

        /* 2. + f^5_{m-2} - f^5_{m-1} */
        if (m >= 2)   T_pos += pow_f[5][m - 2];
        if (m >= 1)   T_neg += pow_f[5][m - 1];

        /* 3. - f^4_{m-3} - f^4_{m-2} */
        if (m >= 3)   T_neg += pow_f[4][m - 3];
        if (m >= 2)   T_neg += pow_f[4][m - 2];

        /* 4. - f^3_{m-4} + f^3_{m-3} + 5 f^3_{m-2} + 3 f^3_{m-1} */
        if (m >= 4)   T_neg += pow_f[3][m - 4];
        if (m >= 3)   T_pos += pow_f[3][m - 3];
        if (m >= 2)   T_pos += (seq_t)5 * pow_f[3][m - 2];
        if (m >= 1)   T_pos += (seq_t)3 * pow_f[3][m - 1];

        /* 5. + f^2_{m-4} + 3 f^2_{m-3} + 3 f^2_{m-2} + f^2_{m-1} */
        if (m >= 4)   T_pos += pow_f[2][m - 4];
        if (m >= 3)   T_pos += (seq_t)3 * pow_f[2][m - 3];
        if (m >= 2)   T_pos += (seq_t)3 * pow_f[2][m - 2];
        if (m >= 1)   T_pos += pow_f[2][m - 1];

        /* 6. - f_{m-4} - 4 f_{m-3} - 6 f_{m-2} - 4 f_{m-1} */
        if (m >= 4)   T_neg += pow_f[1][m - 4];
        if (m >= 3)   T_neg += (seq_t)4 * pow_f[1][m - 3];
        if (m >= 2)   T_neg += (seq_t)6 * pow_f[1][m - 2];
        if (m >= 1)   T_neg += (seq_t)4 * pow_f[1][m - 1];

        /* 7. + binomial(5,m) for m ≤ 5 */
        if (m < 6)    T_pos += bin5[m];

        /* a(m) = T_pos - T_neg (result is always non‑negative) */
        a_vals[m] = T_pos - T_neg;

        /* Update powers of f for later indices. */
        pow_f[1][m] = a_vals[m];   /* f itself */

        for (int k = 2; k <= 6; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t i = 0; i <= m; ++i)
                sum += a_vals[i] * pow_f[k - 1][m - i];
            pow_f[k][m] = sum;
        }
    }

    max_n = n;
    return a_vals[n];
}
