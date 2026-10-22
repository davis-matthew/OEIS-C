/*
 * A361277  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = n! * Sum_{j=0..n} binomial(k*j,n-j)/j!.
 * https://oeis.org/A361277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the square array T(n,k) where T(n,k) = n! *
 *            Σ_{j=0..n} binomial(k*j, n-j)/j!. a(n) is the term on the
 *            antidiagonal n read downwards: let S = floor((sqrt(1+8n)-1)/2),
 *            i = n - S(S+1)/2, j = S - i. Then a(n) = T(i,j)
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, factorial, triangular_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/triangular_limit.h"

/* Compute T(n,k) using the definition via summation. */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (n == 0) return 1;
    seq_t fact_n = factorial(n);      /* n! */
    seq_t sum = 0;
    seq_t factor = fact_n;             /* current factor = n! / r! */
    for (idx_t r = 1; r <= n; ++r) {
        seq_t bin = binomial(k * r, n - r);
        if (bin != 0) {
            seq_t term = factor * bin;
            sum += term;
        }
        if (r < n)
            factor /= (r + 1);        /* move to next r: n!/(r+1)! */
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t S = triangular_limit(n);
    idx_t i = n - (S * (S + 1)) / 2;
    idx_t j = S - i;
    return compute_T(i, j);
}
