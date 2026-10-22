/*
 * A362079  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = [x^n] 1/(1 - x*(1+x)^n)^k.
 * https://oeis.org/A362079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where i+j = s with s the largest integer *
 *            satisfying s*(s+1)/2 <= n. i = n - s*(s+1)/2 and j = s - i. *
 *            T(n,k) = Σ_{j=0..n} binomial(j+k-1, j) * binomial(n*j, n-j), *
 *            with T(0,0)=1, T(n,0)=0 for n>0, and T(0,k)=1 for any k
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Find the antidiagonal index s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
static idx_t diagonal_of(idx_t n)
{
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;
    return s;
}

/* Compute T(n,k) using the binomial formula. */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (k == 0)          /* T(n,0) = [x^n] 1 = 1 if n==0 else 0 */
        return (n == 0) ? (seq_t)1 : (seq_t)0;

    if (n == 0)          /* T(0,k) = 1 for any k */
        return (seq_t)1;

    seq_t sum = 0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t term1 = binomial(j + k - 1, j);
        seq_t term2 = binomial(n * j, n - j);
        sum = sum + term1 * term2;
    }
    return sum;
}

/* Public function: a(n) as defined in the OEIS entry. */
seq_t a(idx_t n)
{
    idx_t s = diagonal_of(n);
    idx_t offset = s * (s + 1) / 2;
    idx_t i = n - offset;   /* row index within diagonal */
    idx_t j = s - i;        /* column index within diagonal */
    return compute_T(i, j);
}
