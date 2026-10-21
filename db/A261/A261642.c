/*
 * A261642  Triangle, read by rows, where T(n,k) = (k^2 + k)^(n-k) for k=1..n
 *          and n>=1.
 * https://oeis.org/A261642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where T(row,col) = (col^2 + col)^(row-col) and
 *            the triangle is read by rows, with offset 1 (a(1) = 1)
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t N)
{
    if (N < 1) return 0;          /* offset = 1, values before are undefined */

    /* Find the row r such that r*(r+1)/2 >= N. */
    idx_t low = 1, high = 1;
    while ((__int128)high * (high + 1) / 2 < N) high <<= 1;   /* double high */

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((__int128)mid * (mid + 1) / 2 >= N)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column within the row. */
    idx_t prev = (r - 1) * r / 2;      /* sum of lengths of earlier rows   */
    idx_t k   = N - prev;              /* 1‑based column index              */

    /* Compute base = k^2 + k safely. */
    __int128 base128 = (__int128)k * k + k;
    seq_t base = (seq_t)base128;

    /* Exponent e = r - k.  e fits in idx_t. */
    idx_t e = r - k;

    /* Use the provided big_pow routine to compute base^e. */
    return big_pow(base, (seq_t)e);
}
