/*
 * A306706  Expansion of Sum_{k>=0} x^(k*(k+1)/2) / Product_{j=1..k} (1 +
 *          x^j)^j.
 * https://oeis.org/A306706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in Sum_{k>=0} x^{k*(k+1)/2} /
 *            Product_{j=1..k} (1 + x^j)^j (OEIS A306706)
 * Range:     i8 n <= 31, i16 n <= 57, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* allocate an array of seq_t with n+1 elements */
static seq_t *alloc_array(idx_t n)
{
    return (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
}

/* free an array allocated by alloc_array */
static void free_array(seq_t *p)
{
    free(p);
}

/* Recompute a(0..N) and store in cache */
static void recompute(idx_t N, seq_t **cached_ans, idx_t *cached_N)
{
    /* space for answer and two intermediate polynomials */
    seq_t *ans   = alloc_array(N);
    seq_t *f_curr = alloc_array(N);
    seq_t *f_next = alloc_array(N);

    /* initialise */
    for (idx_t i = 0; i <= N; ++i) {
        ans[i]   = 0;
        f_curr[i] = 0;
        f_next[i] = 0;
    }
    f_curr[0] = (seq_t)1;     /* k = 0 */

    /* largest k such that k(k+1)/2 <= N */
    idx_t K = 0;
    while ((K + 1) * (K + 2) / 2 <= N) ++K;

    for (idx_t k = 0; k <= K; ++k) {
        idx_t shift = k * (k + 1) / 2;
        idx_t max_e = N - shift;
        for (idx_t e = 0; e <= max_e; ++e)
            ans[shift + e] += f_curr[e];

        if (k == K) break;   /* no further multiplication */

        /* compute coefficients of (1 + x^{k+1})^{-(k+1)} */
        idx_t j = k + 1;
        idx_t max_t = N / j;
        seq_t *coeff = alloc_array(max_t + 1);

        coeff[0] = (seq_t)1;
        for (idx_t t = 1; t <= max_t; ++t) {
            /* coeff[t] = coeff[t-1] * (j + t - 1) / t, then negate */
            seq_t c = coeff[t - 1] * (seq_t)(j + t - 1) / (seq_t)t;
            c = -c;                       /* sign alternates each step */
            coeff[t] = c;
        }

        /* convolution: f_next = f_curr * coeff (spacing j) */
        for (idx_t i = 0; i <= N; ++i) f_next[i] = 0;
        for (idx_t e = 0; e <= N; ++e) {
            seq_t val = f_curr[e];
            if (val == 0) continue;
            idx_t max_t2 = (N - e) / j;
            if (max_t2 > max_t) max_t2 = max_t;
            for (idx_t t = 0; t <= max_t2; ++t)
                f_next[e + j * t] += val * coeff[t];
        }

        free_array(coeff);

        /* swap for next iteration */
        seq_t *tmp = f_curr;
        f_curr = f_next;
        f_next = tmp;
    }

    /* free temporary arrays */
    free_array(f_next);
    free_array(f_curr);

    /* replace old cache */
    if (*cached_ans) free_array(*cached_ans);
    *cached_ans = ans;
    *cached_N = N;
}

/* static cache */
static seq_t *cached_ans = NULL;
static idx_t cached_N = 0;

/* Public function requested by the build */
seq_t a(idx_t n)
{
    if (cached_ans == NULL || cached_N < n)
        recompute(n, &cached_ans, &cached_N);
    return cached_ans[n];
}
