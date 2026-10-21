/*
 * A261633  Expansion of Product_{k>=0} (1+x^(3*k+1))^3.
 * https://oeis.org/A261633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0} (1 + x^(3k+1))^3
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: polynomial multiplication with a three‑term factor
 *  (1 + 3x^m + 3x^(2m) + x^(3m)), iterating m = 1,4,7,... up to n. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* local variables */
    size_t N = (size_t)n;                  /* array bounds fit in size_t */
    seq_t *dp0 = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *dp1 = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp0 || !dp1) { /* fall back to zero if allocation fails */
        if (dp0) free(dp0);
        if (dp1) free(dp1);
        return (seq_t)0;
    }

    /* initialise dp0: only constant term is 1 */
    for (size_t i = 0; i <= N; ++i)
        dp0[i] = (seq_t)0;
    dp0[0] = (seq_t)1;

    /* iterate over factors (1 + 3x^m + 3x^(2m) + x^(3m)) */
    for (idx_t k = 0; ; ++k) {
        idx_t m = 3 * k + 1;
        if ((idx_t)m > n) break;

        /* copy old polynomial to dp1 (will be overwritten) */
        for (size_t i = 0; i <= N; ++i)
            dp1[i] = dp0[i];

        /* add contributions of the factor */
        for (idx_t i = m; i <= n; ++i)
            dp1[(size_t)i] += (seq_t)3 * dp0[(size_t)(i - m)];
        for (idx_t i = 2 * m; i <= n; ++i)
            dp1[(size_t)i] += (seq_t)3 * dp0[(size_t)(i - 2 * m)];
        for (idx_t i = 3 * m; i <= n; ++i)
            dp1[(size_t)i] += dp0[(size_t)(i - 3 * m)];

        /* swap pointers for next iteration */
        seq_t *tmp = dp0;
        dp0 = dp1;
        dp1 = tmp;
    }

    seq_t result = dp0[N];
    free(dp0);
    free(dp1);
    return result;
}
