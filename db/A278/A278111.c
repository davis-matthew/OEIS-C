/*
 * A278111  Triangle T(n,k) = floor(2n^2/k) for 1 <= k <= 2n^2, read by rows.
 * https://oeis.org/A278111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = floor(2*r² / k), where r is the row number such that
 *            S(r‑1) < n ≤ S(r) and S(m) = m(m+1)(2m+1)/3, while k = n –
 *            S(r‑1) is the column index within that row
 * Range:     u8 n <= 826, u16 n <= 9171, u32 n <= 9171, u64 n <= 9171,
 *            u128 n <= 9171, big any n
 * Helpers:   S
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/S.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t for comparison with S() */
    seq_t target = (seq_t)n;

    /* find r with S(r‑1) < target ≤ S(r) */
    idx_t low = 1, high = 1;
    while (S(high) < target)
        high <<= 1;   /* double until sufficient */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (S(mid) >= target)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* column index k = n – S(r‑1) */
    seq_t diff = target - S(r - 1);
    idx_t k = (idx_t)diff;   /* fits in idx_t (≤ 2r²) */

    /* compute floor(2*r² / k) */
    unsigned __int128 num = (unsigned __int128)r * r * 2;
    return (seq_t)(num / k);
}
