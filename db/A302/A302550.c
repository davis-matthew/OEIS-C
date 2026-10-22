/*
 * A302550  Expansion of Sum_{k>=1} (-1 + Product_{j>=1} (1 + x^(k*j))^j).
 * https://oeis.org/A302550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A026007(d) where A026007(d) is the coefficient
 *            of x^d in ∏_{j≥1} (1 + x^j)^j (the term with part size j may
 *            appear 0…j times). The sequence of f(d)=A026007(d) can be
 *            computed by a bounded‑knapsack DP. a(n) is then obtained by
 *            summing f(d) over all divisors d of n
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* external type declarations are supplied by the build environment */
extern seq_t a(idx_t n)
{
    /* zero index is unused; sequence starts at n=1 */
    if (n < 1) return (seq_t)0;

    /* cached values of f(d)=A026007(d) for all d up to the largest n processed so far */
    static seq_t *f = NULL;
    static idx_t f_size = 0;   /* current size of f (number of elements) */

    /* If we need values beyond the current cache, recompute up to n */
    if (n >= f_size)
    {
        /* allocate temporary DP arrays of size n+1 */
        idx_t N = n;                     /* local copy for speed */
        seq_t *dp = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
        seq_t *newdp = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
        if (!dp || !newdp) /* allocation failure (should not happen) */
        {
            free(dp); free(newdp);
            return (seq_t)0;
        }

        dp[0] = (seq_t)1;                 /* empty partition */

        /* Process parts j = 1 .. n */
        for (idx_t j = 1; j <= N; ++j)
        {
            /* maximum number of copies of part j that can be used without exceeding weight N */
            idx_t max_t = N / j;
            if (max_t > j) max_t = j;      /* cannot use more than j copies */

            /* compute binomial coefficients C(j, t) for t = 0 .. max_t */
            seq_t *coef = (seq_t*)malloc((size_t)(max_t + 1) * sizeof(seq_t));
            if (!coef) { free(dp); free(newdp); return (seq_t)0; }
            coef[0] = (seq_t)1;
            for (idx_t t = 1; t <= max_t; ++t)
            {
                /* coef[t] = coef[t-1] * (j - t + 1) / t  (exact division) */
                coef[t] = (coef[t-1] * (seq_t)(j - t + 1)) / (seq_t)t;
            }

            /* zero the newdp array up to index N */
            memset(newdp, 0, (size_t)(N + 1) * sizeof(seq_t));

            /* convolution: dp * (sum_{t=0}^{max_t} C(j,t) x^{t*j})  */
            for (idx_t tot = 0; tot <= N; ++tot)
            {
                seq_t base = dp[tot];
                if (base == (seq_t)0) continue;

                /* t = 0 term (no copy of part j) */
                newdp[tot] += base;

                /* t >= 1 terms */
                for (idx_t t = 1; t <= max_t; ++t)
                {
                    idx_t idx = tot + t * j;
                    if (idx > N) break;
                    newdp[idx] += base * coef[t];
                }
            }

            /* prepare for next iteration: dp becomes newdp, newdp swapped */
            seq_t *tmp = dp;
            dp = newdp;
            newdp = tmp;

            free(coef);
        }

        /* now dp holds the coefficients f[0..N] */
        /* free the old cached array if any */
        free(f);
        f = dp;                 /* keep this array for future access */
        f_size = N + 1;         /* array now contains indices 0..N */
        free(newdp);             /* the old dp array is no longer needed */
    }

    /* Compute a(n) as sum of f(d) over all divisors d of n */
    seq_t result = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            result += f[d];
            idx_t other = n / d;
            if (other != d) result += f[other];
        }
    }
    return result;
}
