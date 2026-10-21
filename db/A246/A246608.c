/*
 * A246608  Expansion of phi(-q) * phi(-q^4)^4 in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A246608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in * φ(-q) · φ(-q⁴)⁴ * = η(q)² ·
 *            η(q⁴)⁸ / (η(q²) · η(q⁸)⁴) * * The product can be written as *
 *            \displaystyle \prod_{k≥1}\bigl(1 - q^k\bigr)^{e_k} * where the
 *            exponent e_k is * e_k = 2 + 8·[4|k] – 1·[2|k] – 4·[8|k], * which
 *            yields the small values e_k ∈ {1,2,5,9}. * * The...
 * Range:     i8 n <= 20, i16 n <= 696, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static state for the pre‑computed table   */
static seq_t *dp = NULL;              /* coefficients a(0)…a(N)   */
static idx_t max_computed = (idx_t)-1;/* largest n for which dp is valid */

/* ------------------------------------------------------------------ */
/*  Compute all coefficients a(0)…a(N) and store them in dp.          */
/* ------------------------------------------------------------------ */
static void compute_all(idx_t N)
{
    size_t sz = (size_t)N + 1;
    seq_t *new_dp = (seq_t*)calloc(sz, sizeof(seq_t));
    new_dp[0] = 1;                    /* a(0) = 1 */

    /* temporary array for the contributions of the current factor */
    seq_t *delta = (seq_t*)calloc(sz, sizeof(seq_t));

    for (idx_t k = 1; k <= N; ++k) {
        /* exponent e_k for the factor (1 - q^k)^{e_k} */
        int e = 2;
        if (k % 4 == 0) e += 8;
        if (k % 2 == 0) e -= 1;
        if (k % 8 == 0) e -= 4;       /* e ∈ {1,2,5,9} */

        /* zero the temporary array */
        memset(delta, 0, sz * sizeof(seq_t));

        /* compute binomial coefficients C(e,t) for t = 1..e   */
        seq_t binom = 1;               /* C(e,0) = 1 */
        for (int t = 1; t <= e; ++t) {
            binom = binom * (seq_t)(e - t + 1) / (seq_t)t;
            seq_t coef = binom;
            if (t & 1)                /* (-1)^t factor */
                coef = -coef;
            idx_t step = k * (idx_t)t;
            if (step > N) break;

            /* accumulate the effect of this t on the coefficient array   */
            for (idx_t i = step; i <= N; ++i)
                delta[i] += new_dp[i - step] * coef;
        }

        /* add the accumulated delta to new_dp to obtain the updated series   */
        for (idx_t i = 0; i <= N; ++i)
            new_dp[i] += delta[i];
    }

    /* replace the old table and free resources */
    free(dp);
    dp = new_dp;
    max_computed = N;
    free(delta);
}

/* ------------------------------------------------------------------ */
/*  Public function required by the framework                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (dp == NULL || n > max_computed) {
        compute_all(n);
    }
    return dp[n];
}
