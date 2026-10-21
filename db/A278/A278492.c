/*
 * A278492  Square array where row n (n >= 0) gives the numbers remaining
 *          after the n-th round of the Flavius sieve, read by descending
 *          antidiagonals.
 * https://oeis.org/A278492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) : map n to row r and column c by the
 *            descending‑antidiagonal order, then recover the r‑round survivor
 *            by the recurrence idx = c; for (k = r; k >= 1; --k) idx += idx /
 *            k ; finally a(n) = idx + 1. This follows the definition of the
 *            Flavius sieve
 * Range:     u8 n <= 640, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the antidiagonal d such that
       (d+1)(d+2)/2  > n  and (d)(d+1)/2 <= n.
       For the small n (≤ 10000) a simple loop is inexpensive. */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n)
        ++d;

    /* Elements before diagonal d */
    idx_t prev = (d == 0) ? 0 : (d * (d + 1)) / 2;
    /* Position within diagonal d (0‑based) */
    idx_t k = n - prev;

    /* Map to row and column */
    idx_t row = k;
    idx_t col = d - k;

    /* Recover the original index before any elimination.
       idx is 0‑based; final value is idx+1. */
    idx_t idx = col;
    for (idx_t step = row; step > 0; --step)
        idx += idx / step;

    return (seq_t)(idx + 1);
}
