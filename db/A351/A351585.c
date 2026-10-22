/*
 * A351585  Triangle read by rows: T(n,k) = A(k,n-k), 1 <= k < n, 2 <= n,
 *          where A(m,n) is the number of distinct strings consisting of one
 *          X, m+n-1 Y's and m+n-1 Z's in which the X lies to the right of at
 *          least m Y's and at least m Z's.
 * https://oeis.org/A351585
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r and k are determined from n by reading the
 *            triangle by rows, with T(r,k) = (r - k) * C(2*r - 1, r) - 2*k*(r
 *            - k)*C(2*k - 1, k)*C(2*(r - k) - 1, r - k)/r (C(n,k) is the
 *            binomial coefficient)
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;           /* behaviour below the offset is irrelevant */

    /* Find the row r and column k that correspond to position n
       in the flattened triangle (offset by 2 because a(2) is first). */
    idx_t p0 = n - 2;          /* zero‑based offset within the triangle */
    idx_t r = 2;               /* current row number */
    idx_t count_before = 0;    /* number of terms before the current row */

    while (count_before + (r - 1) <= p0) {
        count_before += r - 1;
        ++r;
    }

    idx_t k = (p0 - count_before) + 1;   /* column index (1‑based) */

    /* Compute the value using the closed form */
    seq_t term1 = (seq_t)(r - k) *
                  binomial((idx_t)(2 * r - 1), r);

    seq_t term2 = (seq_t)2 * (seq_t)k * (seq_t)(r - k) *
                  binomial((idx_t)(2 * k - 1), k) *
                  binomial((idx_t)(2 * (r - k) - 1), r - k);

    term2 = term2 / (seq_t)r;   /* exact division */

    return term1 - term2;
}
