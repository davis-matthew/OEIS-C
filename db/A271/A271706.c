/*
 * A271706  Triangle read by rows: T(n, k) = Sum_{j=0..n} C(-j-1, -n-1)*L(j,
 *          k), L the unsigned Lah numbers A271703, for n >= 0 and 0 <= k <=
 *          n.
 * https://oeis.org/A271706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r, k) where r,k satisfy n = r*(r+1)/2 + k (with 0 ≤ k ≤
 *            r). T(r, k) = (-1)^(r-k) * binomial(r, k) * sum_{s=0}^{r-k}
 *            (k-r)_s * (k)_s / s! with (x)_0 = 1. (Indices start at 0, as
 *            required by the OEIS entry.)
 * Range:     i8 n <= 21, i16 n <= 37, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n)
        ++r;
    --r; /* now r is the correct row */
    /* Column k within the row */
    idx_t k = n - (r * (r + 1)) / 2;

    /* Compute T(r, k) according to the above definition. */
    seq_t binom = binomial(r, k);

    /* Sum the series.  Use s for the summation index. */
    idx_t m = r - k;           /* upper limit for s */
    seq_t term = 1;            /* (k-r)_0 * (k)_0 / 0! = 1 */
    seq_t sum  = 1;            /* include s = 0 term */

    for (idx_t s = 1; s <= m; ++s) {
        /* (k-r)_s = (k-r)_{s-1} * (k-r + s - 1)   */
        /* (k)_s    = (k)_{s-1}    * (k + s - 1)    */
        long long a = (long long)k - (long long)r;   /* k-r (negative or zero) */
        long long b = (long long)k;                  /* k */
        term = (term
                * (seq_t)(a + (long long)s - 1)   /* k-r + s - 1 */
                * (seq_t)(b + (long long)s - 1)   /* k + s - 1 */
                ) / (seq_t)s;                     /* divide by s! factor incrementally */
        sum += term;
    }

    seq_t result = binom * sum;

    /* Apply the overall sign factor (-1)^(r-k). */
    if ((m & 1) != 0)
        result = -result;

    return result;
}
