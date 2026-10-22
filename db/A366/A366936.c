/*
 * A366936  Square array T(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=1..n} (-1)^j*j^k*floor(n/j).
 * https://oeis.org/A366936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(p, d-p) where d = smallest integer with d*(d+1)/2 >= n,
 *            p = n - d*(d-1)/2 (1 ≤ p ≤ d). T(n,k) = Σ_{j=1..n} (-1)^j * j^k
 *            * floor(n/j)
 * Range:     i8 n <= 30, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the sequence term a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    /* Determine diagonal d and position p within that diagonal. */
    idx_t d = 0;
    while (d * (d + 1) / 2 < n) {
        ++d;
    }
    idx_t prev = (d - 1) * d / 2;
    idx_t p = n - prev;           /* 1‑based position within diagonal */
    idx_t k = d - p;              /* column index 0‑based */

    /* Compute T(p, k) = Σ_{j=1..p} (-1)^j * j^k * floor(p/j). */
    seq_t sum = 0;
    for (idx_t j = 1; j <= p; ++j) {
        /* Compute j^k. */
        seq_t pow = 1;
        for (idx_t t = 0; t < k; ++t) {
            pow *= (seq_t)j;
        }
        /* floor(p/j) is integer division. */
        seq_t fl = (seq_t)(p / j);

        if (j & 1ULL) {
            sum -= pow * fl;    /* odd j: multiply by (-1) */
        } else {
            sum += pow * fl;    /* even j */
        }
    }
    return sum;
}
