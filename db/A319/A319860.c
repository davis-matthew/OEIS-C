/*
 * A319860  Expansion of Product_{k>0} (1 - 2*k*x^(2*k))/(1 +
 *          (2*k-1)*x^(2*k-1)).
 * https://oeis.org/A319860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 - 2*k*x^(2*k)) / (1 +
 *            (2*k-1)*x^(2*k-1))
 * Range:     i8 n <= 14, i16 n <= 31, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation multiplies the series successively by each
   factor.  Each factor is written as (1 + (-c)*x^m)^(-1) * (1 - 2k*x^(2k)),
   where c = 2*k-1 and m = 2*k-1.  The geometric series part
   (1 + (-c)*x^m)^(-1) satisfies the recurrence
          h[j] = f[j] + (-c) * h[j-m]    (j ≥ m)
   which is used to build an intermediate series h.  The final
   series after applying the numerator factor is
          f_new[j] = h[j] - (2*k) * h[j-2*k]   (j ≥ 2*k)
   The algorithm runs in O(n * k_max) ≈ O(n^2/2) time and
   uses only two n+1 sized arrays. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Quick return for n == 0 */
    if (n == 0) return (seq_t)1;

    size_t sz = (size_t)(n + 1);
    seq_t *f  = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *h  = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!f || !h)
    {
        free(f);
        free(h);
        return (seq_t)0;  /* memory allocation failure fallback */
    }

    /* initialise f to 1 + 0*x + 0*x^2 + ... */
    memset(f, 0, sz * sizeof(seq_t));
    f[0] = (seq_t)1;

    /* maximum k needed: terms with 2k-1 ≤ n or 2k ≤ n are relevant.
       Using k_max = (n+1)/2 includes all needed factors. */
    idx_t k_max = (n + 1) / 2;

    for (idx_t k = 1; k <= k_max; ++k)
    {
        seq_t c = (seq_t)(2 * k - 1);          /* c = 2k-1   */
        idx_t  m = 2 * k - 1;                  /* step size  */
        idx_t  shift = 2 * k;                  /* 2k for numerator shift */

        /* copy current series into h */
        for (idx_t i = 0; i <= n; ++i) h[i] = f[i];

        /* h[j] = h[j] + (-c) * h[j-m]  for j >= m   */
        seq_t negc = -(c);
        for (idx_t j = m; j <= n; ++j)
            h[j] = h[j] + negc * h[j - m];

        /* f_new[j] = h[j] - shift * h[j-shift]  for j >= shift */
        for (idx_t j = 0; j <= n; ++j)
        {
            seq_t val = h[j];
            if (j >= shift)
                val = val - (seq_t)shift * h[j - shift];
            f[j] = val;
        }
    }

    seq_t result = f[n];
    free(f);
    free(h);
    return result;
}
