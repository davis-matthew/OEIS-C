/*
 * A263184  E.g.f. is the series C(x) such that C(x) + i*S(x) = 1 + i *
 *          Integral C(x)/(C(x) + i*S(x)) dx, even powers only, where i^2 =
 *          -1.
 * https://oeis.org/A263184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence for the series C(x) and S(x) defined by
 *   C + i S = 1 + i ∫ C/(C+iS) dx  (only even powers),
 *   C^2 - S^2 = 1,
 *   C' = C S/(C^2 + S^2),  S' = C^2/(C^2 + S^2).
 *
 * Let c_n and s_n be the coefficients of C(x) and S(x) in a^k/k! form.
 * Then for n ≥ 0
 *
 *   s_{n+1} = c_n
 *           - Σ_{k=1}^{n} binom(n,k)[ c_k s_{n-k+1} + s_k c_{n-k+1} ]
 *
 *   c_{n+1} = - Σ_{k=1}^{n} binom(n,k)[ c_k c_{n-k+1} - s_k s_{n-k+1} ]
 *
 * Initial values:  c_0 = 1, c_1 = 0,  s_0 = 0, s_1 = 1.
 *
 * The required sequence is a(n) = c_{2n}.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static memoisation buffers */
    static seq_t *c = NULL;      /* coefficients of C(x)              */
    static seq_t *s = NULL;      /* coefficients of S(x)              */
    static size_t cap = 0;       /* allocated array capacity         */
    static idx_t last_idx = 0;   /* highest index already computed   */

    /* required size – we need indices up to 2*n, plus two extra slots
       for the next‑step computation.  The array must contain indices
       0 … idx_max+1 inclusive. */
    idx_t idx_max = 2 * n;           /* last coefficient index needed   */
    size_t new_cap = (size_t)idx_max + 2;  /* room for indices 0 .. idx_max+1 */

    /* initialise arrays on first call */
    if (c == NULL) {
        size_t init_cap = 2;                     /* indices 0 and 1 */
        c = (seq_t *)malloc(init_cap * sizeof(seq_t));
        s = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (c == NULL || s == NULL) abort();

        /* zero the new entries */
        for (size_t i = 0; i < init_cap; ++i) {
            c[i] = 0;
            s[i] = 0;
        }

        /* base values */
        c[0] = (seq_t)1;   /* C(0) = 1  */
        /* c[1] = 0 already */
        /* s[0] = 0 already */
        /* s[1] will be produced by the recurrence when needed */

        cap = init_cap;
        last_idx = 0;
    }

    /* grow buffers if necessary */
    if (new_cap > cap) {
        seq_t *c_new = (seq_t *)realloc(c, new_cap * sizeof(seq_t));
        seq_t *s_new = (seq_t *)realloc(s, new_cap * sizeof(seq_t));
        if (c_new == NULL || s_new == NULL) abort();

        /* zero the newly allocated entries */
        for (size_t i = cap; i < new_cap; ++i) {
            c_new[i] = 0;
            s_new[i] = 0;
        }

        c = c_new;
        s = s_new;
        cap = new_cap;
    }

    /* if already computed up to idx_max return directly */
    if (idx_max <= last_idx) {
        return c[2 * n];
    }

    /* compute new coefficients up to idx_max (if not already computed) */
    for (idx_t n_i = last_idx; n_i <= idx_max - 1; ++n_i) {
        /* current binomial coefficient binom(n_i, k) in 128‑bit to avoid overflow */
        __int128 bin = 1;
        seq_t ss_sum = 0;       /* running sum for (1) */
        seq_t cc_sum = 0;       /* running sum for (2) */

        for (idx_t k = 1; k <= n_i; ++k) {
            bin = bin * (n_i - k + 1) / k;          /* exact binomial */
            seq_t bin_t = (seq_t)bin;

            seq_t c_k = c[k];
            seq_t s_k = s[k];
            seq_t c_nk1 = c[n_i - k + 1];
            seq_t s_nk1 = s[n_i - k + 1];

            /* term for s_{n+1} recurrence (1) */
            ss_sum += bin_t * (c_k * s_nk1 + s_k * c_nk1);

            /* term for c_{n+1} recurrence (2) */
            cc_sum += bin_t * (c_k * c_nk1 - s_k * s_nk1);
        }

        /* compute s[n_i+1] and c[n_i+1] */
        s[n_i + 1] = c[n_i] - ss_sum;   /* from (1) */
        c[n_i + 1] = -cc_sum;           /* from (2) */
    }

    /* update the highest index computed so far */
    last_idx = idx_max;

    /* return the requested term a(n) = c_{2n} */
    return c[2 * n];
}
