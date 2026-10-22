/*
 * A304787  Expansion of Product_{k>=1} (1 + x^k)^(binomial(2*k,k)/(k+1)).
 * https://oeis.org/A304787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + x^k)^{Catalan(k)}
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             (Catalan(k) = binomial(2k,k)/(k+1)) */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/catalan.h"

/* Compute a(n) for OEIS A304787. */
seq_t a(idx_t n) {
    /* Handle the trivial case. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate temporary arrays.  All arrays are sized n+1. */
    seq_t *pos = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *neg = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *r   = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *dp  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!pos || !neg || !r || !dp)
        return (seq_t)0;   /* error path: return 0 on allocation failure */

    /* STEP 1: Compute the auxiliary array r[j] = Σ_{k|j} Catalan(k)*k*sgn(j/k)
       where sgn(m) = +1 if m is odd, –1 if m is even.  This is achieved by
       accumulating positive and negative parts separately to avoid
       intermediate negativity. */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t catk  = catalan(k);        /* Catalan(k) */
        seq_t coeff = catk * (seq_t)k;   /* Catalan(k)*k  (constant for this k) */

        for (idx_t m = 1, j = k; j <= n; ++m, j += k) {
            /* m = j / k  (integer) */
            if (m & 1U)          /* odd m → positive contribution */
                pos[j] += coeff;
            else                 /* even m → negative contribution */
                neg[j] += coeff;
        }
    }

    /* Convert positive/negative parts into final r[j] = pos[j] – neg[j] (always ≥0). */
    for (idx_t j = 1; j <= n; ++j) {
        r[j] = pos[j];
        /* pos[j] ≥ neg[j] for all j, so the subtraction is safe. */
        r[j] -= neg[j];
    }

    /* Free intermediate arrays that are no longer needed. */
    free(pos);
    free(neg);

    /* STEP 2: Compute the sequence a[n] using the recurrence derived from
       the logarithmic derivative:  n·a[n] = Σ_{j=1}^n a[n-j]·r[j]. */
    dp[0] = (seq_t)1;   /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= i; ++j) {
            sum += dp[i - j] * r[j];
        }
        dp[i] = sum / i;   /* exact division by i */
    }

    seq_t result = dp[n];
    free(r);
    free(dp);
    return result;
}
