/*
 * A342847  Value of r in best integer approximation r^s to n! with s >= r.
 * https://oeis.org/A342847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = value of r that minimizes |n! - r^s| with s >= r; ties
 *            are resolved by choosing the smallest possible s
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <float.h>

seq_t a(idx_t n)
{
    /* Handle offset n = 0; 0! = 1. */
    if (n == 0ULL)
        return 1;

    /* Compute ln(n!) = sum_{i=2}^{n} ln(i).  Use long double for safety. */
    long double ln_fact = 0.0L;
    for (idx_t i = 2; i <= n; ++i)
        ln_fact += logl((long double)i);

    /* Constants for comparisons. */
    const long double eps  = 1e-15L;
    const long double INF  = INFINITY;

    long double best_g = INF;     /* best value of |exp(d) - 1| */
    seq_t      best_r = 1;        /* best r found so far */
    idx_t      best_s = 1;        /* corresponding s (for tie‑breakers) */

    /* ---- r = 1 ------------------------------------------------------- */
    long double exp_neg_ln_fact = (ln_fact > 700.0L) ? 0.0L : expl(-ln_fact);
    long double g = 1.0L - exp_neg_ln_fact;   /* |1 - 1/n!| */
    if (g < best_g - eps || (fabsl(g - best_g) <= eps && 1 < best_s))
    {
        best_g = g;
        best_r = 1;
        best_s = 1;
    }

    /* ---- r >= 2 ----------------------------------------------------- */
    for (idx_t r = 2; r <= n; ++r)
    {
        long double ln_r = logl((long double)r);

        /* Real logarithmic exponent s0. */
        long double s_real = ln_fact / ln_r;

        /* Candidate exponents: floor(s_real) (but not less than r) and one above. */
        idx_t s1 = (idx_t)floorl(s_real);
        if (s1 < r) s1 = r;
        idx_t s2 = s1 + 1;

        /* Compute |exp(d) - 1| for s1. */
        long double d1 = (long double)s1 * ln_r - ln_fact;
        long double g1;
        if (!isfinite(d1) || d1 > 400.0L)          g1 = INF;
        else if (d1 < -400.0L)                     g1 = 1.0L;   /* exp(d) ≈ 0 */
        else {
            long double expd = expl(d1);
            g1 = (d1 >= 0.0L) ? expd - 1.0L : 1.0L - expd;
        }

        /* Compute |exp(d) - 1| for s2. */
        long double d2 = (long double)s2 * ln_r - ln_fact;
        long double g2;
        if (!isfinite(d2) || d2 > 400.0L)          g2 = INF;
        else if (d2 < -400.0L)                     g2 = 1.0L;
        else {
            long double expd = expl(d2);
            g2 = (d2 >= 0.0L) ? expd - 1.0L : 1.0L - expd;
        }

        /* Choose best s for this r. */
        long double g_best_r = g1;
        idx_t      s_best_r  = s1;
        if (g2 < g_best_r - eps || (fabsl(g2 - g_best_r) <= eps && s2 < s_best_r))
        {
            g_best_r = g2;
            s_best_r = s2;
        }

        /* Update global best. Tie-breaking by smaller s. */
        if (g_best_r < best_g - eps ||
            (fabsl(g_best_r - best_g) <= eps && s_best_r < best_s))
        {
            best_g = g_best_r;
            best_r = (seq_t)r;
            best_s = s_best_r;
        }
    }

    return best_r;
}
