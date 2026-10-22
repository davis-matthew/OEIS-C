/*
 * A338435  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) = n!*LaguerreL(n, -k*n).
 * https://oeis.org/A338435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where i+j = s is the antidiagonal, T(i,j) = i! *
 *            LaguerreL(i, -j*i) = sum_{k=0..i} (i*j)^k * (i-k)! *
 *            binom(i,k)^2
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

static seq_t compute_T(idx_t n, idx_t k)
{
    if (n == 0)                 /* T(0, k) = 1 */
        return 1;

    /* kn = k*n as seq_t */
    seq_t kn = (seq_t)n * (seq_t)k;

    /* factorial(n) may overflow seq_t for large n, but we only need exact
       results for n small enough that the final term fits in seq_t
       (which is true for all cases tested). */
    seq_t fact = factorial(n);

    seq_t comb = 1;             /* binom(n,0) */
    seq_t power = 1;            /* (kn)^0 */
    seq_t result = 0;

    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = power;
        term *= fact;
        term *= comb;
        term *= comb;           /* combine * comb * comb */
        result += term;

        if (j == n)            /* finished */
            break;

        /* Prepare for next j */
        power *= kn;                       /* (kn)^(j+1) */
        comb *= (seq_t)(n - j);             /* comb * (n-j) */
        comb /= (j + 1);                    /* / (j+1)  */
        fact /= (seq_t)(n - j);             /* (n-(j+1))!  */
    }

    return result;
}

seq_t a(idx_t n)
{
    /* Find diagonal s such that sum_{t=0}^{s-1} (t+1) <= n < sum_{t=0}^{s} (t+1) */
    idx_t s = 0;
    idx_t cum = 0;
    while (cum + (s + 1) <= n) {
        cum += s + 1;
        ++s;
    }
    /* offset within this diagonal (0-based) */
    idx_t offset = n - cum;
    idx_t row = offset;          /* i */
    idx_t col = s - offset;      /* j */

    return compute_T(row, col);
}
