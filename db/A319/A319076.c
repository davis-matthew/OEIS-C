/*
 * A319076  Square array T(n,k) read by antidiagonal upwards in which column k
 *          lists the partial sums of the powers of the k-th prime, n >= 0, k
 *          >= 1.
 * https://oeis.org/A319076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where T(r,k)=∑_{j=0..r} prime_k^j, * read by
 *            antidiagonals upwards in the square array of partial sums * of
 *            powers of the k‑th prime
 * Range:     u8 n <= 21, u16 n <= 46, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Find the diagonal s such that s*(s+1)/2 > n (s ≥ 1). */
static inline idx_t find_diagonal(idx_t n)
{
    idx_t s = 0;
    while ((s * (s + 1)) / 2 <= n)
        ++s;
    return s;
}

seq_t a(idx_t n)
{
    /* Locate diagonal and offset within it. */
    idx_t s = find_diagonal(n);
    idx_t prev = (s - 1) * s / 2;   /* elements before this diagonal */
    idx_t offset = n - prev;        /* 0‑based offset inside the diagonal */

    /* Matrix coordinates: row r (0‑based), column k (1‑based). */
    idx_t r = (s - 1) - offset;
    idx_t k = offset + 1;

    /* Obtain the k‑th prime. */
    seq_t prime = (seq_t)get_prime(k);

    /* Compute T(r,k) = 1 + prime + prime² + … + primeʳ. */
    seq_t sum  = (seq_t)1;
    seq_t term = prime;
    for (idx_t i = 0; i < r; ++i)
    {
        sum += term;
        term *= prime;
    }
    return sum;
}
