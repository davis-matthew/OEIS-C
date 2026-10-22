/*
 * A377661  Triangle read by rows: T(n, k) = e*Gamma(n - k + 1, 1)*binomial(n,
 *          k)^2.
 * https://oeis.org/A377661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r(r+1)/2 <= n < (r+1)(r+2)/2, k = n -
 *            r(r+1)/2, and T(r,k) = binomial(r,k) * Σ_{m=0}^{r-k} r!/(k!*m!)
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* locate row r and column k */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2)) / 2 <= n ) ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* compute r! and k! */
    seq_t fact_r = factorial(r);
    seq_t fact_k = factorial(k);
    /* F = r! / k!  (integer) */
    seq_t F = fact_r / fact_k;

    /* sum_{m=0}^{r-k} F / m!   (integer) */
    idx_t d = r - k;
    seq_t term = F;
    seq_t sum  = 0;
    for (idx_t m = 0; m <= d; ++m) {
        sum += term;
        if (m != d) term = term / (m + 1);   /* next term: divide by (m+1) */
    }

    seq_t bin = binomial(r, k);
    return bin * sum;
}
