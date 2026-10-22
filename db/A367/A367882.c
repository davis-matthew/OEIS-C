/*
 * A367882  Table T(n, k) read by downward antidiagonals: T(n, k) =
 *          floor((4*T(n, k-1)+3)/3) starting with T(n, 0) = 4*n.
 * https://oeis.org/A367882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where the pair (i,j) is obtained by reading the
 *            table T by downward anti–diagonals. For a given index n let d =
 *            ⌊ (√(8n+1) − 1) / 2 ⌋, r = n − d(d+1)/2, i = r, j = d − r. Then
 *            T(i,0) = 4*i, T(i,k) = ⌊ (4*T(i,k−1) + 3) / 3 ⌋ (k ≥ 1), and
 *            a(n) = T(i,j). The implementation avoids float...
 * Range:     u8 n <= 105, u16 n <= 562, u32 n <= 2557, u64 n <= 9733,
 *            u128 n <= 9733, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find anti‑diagonal d such that d(d+1)/2 ≤ n < (d+1)(d+2)/2. */
    idx_t d = 0;
    while (1)
    {
        unsigned __int128 t = (unsigned __int128)d * (d + 1) / 2;
        if (t > n) break;
        ++d;
    }
    --d;

    /* Remainder inside the diagonal */
    unsigned __int128 t = (unsigned __int128)d * (d + 1) / 2;
    idx_t r = n - (idx_t)t;

    /* Coordinates (i,j) */
    idx_t i = r;
    idx_t j = d - r;

    /* Compute T(i,j) */
    seq_t val = (seq_t)4 * i;
    for (idx_t k = 1; k <= j; ++k)
    {
        val = (val * 4 + 3) / 3;
    }
    return val;
}
