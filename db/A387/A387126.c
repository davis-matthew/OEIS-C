/*
 * A387126  Triangle read by rows: T(n, k) = (n! / (n - k)!) *
 *          Product_{k=1..n} radical(k), where radical(n) is the product of
 *          distinct prime factors of n, cf. A007947.
 * https://oeis.org/A387126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the triangle read by rows. The triangle
 *            T(n,k) is defined by T(n,k) = (n!/(n-k)!) * ∏_{i=1..n} rad(i),
 *            where rad(i) is the product of distinct prime factors of i. The
 *            sequence a(n) lists the entries of this triangle row‑by‑row (row
 *            0 first, then row 1, …, each row left to...
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"

seq_t a(idx_t n)
{
    /* Find the row r containing the nth term.
       The cumulative number of terms up to row r-1 is r*(r+1)/2.
       We seek the smallest r such that (r+1)(r+2)/2 > n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* The offset within that row. */
    idx_t offset = n - r * (r + 1) / 2;
    idx_t k = offset;           /* k ranges from 0 to r */

    /* Product of radicals up to n.  This factor is common to every
       entry in row n. */
    seq_t prod_rad = 1;
    for (idx_t i = 1; i <= r; ++i)
        prod_rad *= rad(i);

    /* Compute the falling factorial n!/(n-k)! = product_{j=r-k+1..r} j. */
    seq_t comb = 1;
    idx_t start = r - k + 1;     /* if k==0, start = r+1 and loop is skipped */
    for (idx_t j = start; j <= r; ++j)
        comb *= (seq_t)j;

    /* Result of the triangle entry. */
    return prod_rad * comb;
}
