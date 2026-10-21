/*
 * A279394  Triangle read by rows, T(n,m) = sigma_{n-m}(m) for n >= 1, m =
 *          1,2, ..., n.
 * https://oeis.org/A279394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = sigma_{r-m}(m) where r is the smallest integer with
 *            r*(r+1)/2 ≥ n and m = n - r*(r-1)/2
 * Range:     u8 n <= 31, u16 n <= 69, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset 1 */

    /* Find row r: smallest integer with r*(r+1)/2 ≥ n */
    idx_t r = (idx_t)((sqrt((double)8 * n + 1.0) - 1.0) / 2.0);
    if (r * (r + 1) / 2 < n) ++r;

    /* Column m within this row (1‑based) */
    idx_t prev = (r - 1) * r / 2;        /* entries before row r */
    idx_t m = n - prev;                 /* 1 ≤ m ≤ r */

    /* Exponent k = r - m */
    uint8_t k = (uint8_t)(r - m);

    return sigma_k(m, k);
}
