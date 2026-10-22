/*
 * A322596  Square array read by descending antidiagonals (n >= 0, k >= 0):
 *          let b(n,k) = (n+k)!/((n+1)!*k!); then T(n,k) = b(n,k) if b(n,k) is
 *          an integer, and T(n,k) = floor(b(n,k)) + 1 otherwise.
 * https://oeis.org/A322596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   Read the square array T(n,k) = ceil( binomial(n+k,k)/(n+1) ) by
 *            descending antidiagonals. For each index i >= 0, let s be the
 *            unique integer such that s(s+1)/2 <= i < (s+1)(s+2)/2
 *            (triangular numbers). Then let p = i - s(s+1)/2. The pair (n,k)
 *            is n = p, k = s - p . a(i) = ( binomial(s,k) + n ) /...
 * Range:     u8 n <= 39, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t idx)
{
    /* Find the antidiagonal index s such that indices of that diagonal
       contain the given idx.  The loop runs at most O(sqrt(idx)) times.
       Use 128‑bit arithmetic to avoid overflow of the triangular test. */
    idx_t s = 0;
    while (((unsigned __int128)(s + 1) * (s + 2)) / 2 <= idx)
        ++s;

    /* Number of elements in all preceding diagonals */
    idx_t count_before = ((unsigned __int128)s * (s + 1)) / 2;

    /* Local position within diagonal s */
    idx_t p = idx - count_before;        /* 0 <= p <= s */
    idx_t n = p;          /* n-coordinate of the entry */
    idx_t k = s - p;      /* k-coordinate */

    /* Compute ceil( binomial(s,k)/(n+1) ) efficiently:
          ceil(x/y) = (x + y - 1) / y
       Here y = n+1, x = binomial(s,k).  Since (x + y - 1)/(y) truncates
       we can use (x + n)/(n+1). */
    seq_t binom = binomial((idx_t)s, (idx_t)k);
    seq_t denom = (seq_t)(n + 1);
    seq_t result = (binom + n) / denom;

    return result;
}
