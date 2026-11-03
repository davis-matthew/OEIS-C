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
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Largest S with S*(S+1)/2 <= n.  Uses integer arithmetic only. */
static idx_t triangular_limit(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ((__int128)hi * (hi + 1) / 2 <= n)
        hi <<= 1;                     /* find an upper bound */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;                     /* lo-1 is the desired S */
}

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
