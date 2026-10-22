/*
 * A306437  Regular triangle read by rows where T(n,k) is the number of
 *          non-crossing set partitions of {1, ..., n} in which all blocks
 *          have size k.
 * https://oeis.org/A306437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c), where T(r,c)=binomial(r,r/c)/(r-r/c+1) if c|r
 *            else 0. r is the smallest integer with r*(r+1)/2 >= n, and c = n
 *            - r*(r-1)/2, read row‑wise
 * Range:     u8 n <= 29, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the first index of that row is <= n. */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n)
        ++r;

    /* Column within the row (1‑based). */
    idx_t prev = (r - 1) * r / 2;
    idx_t c = n - prev;

    if (c == 0 || (r % c) != 0)
        return (seq_t)0;  /* block size not a divisor of the row size */

    idx_t q = r / c;                 /* q = r / c */
    seq_t num = binomial(r, q);      /* binomial(r, q) */
    seq_t denom = (seq_t)(r - q + 1);
    return num / denom;              /* integer division; result is exact */
}
