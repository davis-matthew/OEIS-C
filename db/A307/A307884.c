/*
 * A307884  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of 1/sqrt(1 + 2*(k-1)*x + ((k+1)*x)^2).
 * https://oeis.org/A307884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the coefficient T(j,k) read by antidiagonals, where
 *            T(j,k) = Sum_{r=0..j} (-k)^r * binomial(j,r)^2
 * Range:     i8 n <= 39, i16 n <= 84, i32 n <= 182, i64 n <= 454,
 *            i128 n <= 1207, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the diagonal m that contains index n.
       The cumulative number of terms up to diagonal m (inclusive)
       is (m+1)(m+2)/2.  Iterate until this exceeds n. */
    idx_t m   = 0;
    idx_t cum = 1;            /* (m+1)(m+2)/2 for m = 0 */
    while (cum <= n) {
        ++m;
        cum += (m + 1);       /* add length of new diagonal m */
    }

    /* Number of terms before diagonal m. */
    idx_t prev = (m * (m + 1)) / 2;

    /* Offset within diagonal m. */
    idx_t pos = n - prev;     /* 0 <= pos <= m */

    /* Antidiagonal enumeration: (j, k) with j = pos, k = m - pos */
    idx_t j = pos;
    idx_t k = m - pos;

    /* Compute T(j,k) = Sum_{r=0..j} (-k)^r * binomial(j,r)^2  */
    seq_t sum   = 0;
    seq_t power = 1;                         /* (-k)^0 */
    for (idx_t r = 0; r <= j; ++r) {
        seq_t b = binomial(j, r);            /* binomial coefficient */
        b = b * b;                            /* square it */
        sum += b * power;
        power *= (seq_t)(-(long long)k);      /* multiply by (-k) for next term */
    }
    return sum;
}
