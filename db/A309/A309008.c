/*
 * A309008  The same array as in A293500, but without the leading row and
 *          column of zeros. See that entry for more information.
 * https://oeis.org/A309008
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(r,c) = (K^N - K^ceil(N/2))/2 with N=r+1, K=c+1 (A293500
 *            without zero row/column), read by antidiagonals from the bottom
 *            row up; a(2)=1
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 46 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A309008_pw(seq_t base, idx_t e)
{
    seq_t r = 1;
    while (e > 0) {
        if (e & 1) r *= base;
        e >>= 1;
        if (e) base *= base;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t pos = n - 1;               /* 1-based position, n >= 2 */
    idx_t d = 1;
    while ((unsigned __int128)d * (d + 1) / 2 < pos) d++;
    idx_t prev = (idx_t)((unsigned __int128)d * (d - 1) / 2);
    idx_t j = pos - prev - 1;        /* 0-based within diagonal */
    idx_t row = d - j;
    idx_t N = row + 1;
    seq_t K = (seq_t)j + 2;
    idx_t h = (N + 1) / 2;
    seq_t ph = A309008_pw(K, h);
    seq_t q = A309008_pw(K, N - h) - 1;
    if (ph % 2 == 0)
        return (ph / 2) * q;
    return ph * (q / 2);
}
