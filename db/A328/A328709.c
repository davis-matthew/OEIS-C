/*
 * A328709  Dirichlet g.f.: Product_{k>=2} ((1 + k^(-s)) / (1 - k^(-s)))^k.
 * https://oeis.org/A328709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A050367(n/d) * A050368(d) The sequence is given
 *            by the Dirichlet series ∏_{k≥2} ((1 + k^{-s})/(1 - k^{-s}))^k.
 *            This implementation evaluates that product directly
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute binomial(n, k) for small arguments (k <= 14).  */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiply by (n - k + i) then divide by i,
           keeping the result integral at each step.  */
        res = (res * ((seq_t)(n - k + i))) / (idx_t)i;
    }
    return res;
}

/* Coefficient of k^{-e s} in ((1 + k^{-s})/(1 - k^{-s}))^k.  */
static seq_t coeff_c(idx_t k, idx_t e)
{
    seq_t sum = 0;
    idx_t max_i = (k < e) ? k : e;
    for (idx_t i = 0; i <= max_i; ++i)
    {
        seq_t a = binom_small(k, i);
        seq_t b = binom_small(k + e - i - 1, e - i);
        sum += a * b;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* defined only for n ≥ 1 */

    /* Allocate two working arrays of size n+1 (index 0 unused).  */
    seq_t *curr = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *next = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!curr || !next) {
        free(curr); free(next);
        return 0;
    }

    /* Start with a(1) = 1.  */
    curr[1] = 1;

    for (idx_t k = 2; k <= n; ++k)
    {
        /* Copy current series into next (to preserve contributions
           from terms with exponent 0 of the current k).  */
        for (idx_t i = 1; i <= n; ++i)
            next[i] = curr[i];

        /* For each exponent e such that k^e ≤ n, convolve the
           corresponding coefficient with the existing series.  */
        idx_t power = k;     /* k^1 */
        idx_t e = 1;
        while (power <= n)
        {
            seq_t coeff = coeff_c(k, e);

            for (idx_t m = power; m <= n; m += power)
                next[m] += coeff * curr[m / power];

            if (power > n / k) break;   /* avoid overflow when computing
                                           the next power of k.  */
            power *= k;
            ++e;
        }

        /* Prepare for next iteration: swap pointers.  */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    seq_t result = curr[n];

    free(curr);
    free(next);
    return result;
}
