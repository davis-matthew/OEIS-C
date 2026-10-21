/*
 * A244911  Table read by antidiagonals: T(n,k) = n*k + T(n-1,k) for n >=1,
 *          T(0,k) = 1.
 * https://oeis.org/A244911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + k*n*(n+1)/2 where k = m - n and m is the unique
 *            integer such that m(m+1)/2 <= n < (m+1)(m+2)/2. The pair (n,k)
 *            are obtained by reading the infinite table T(i,j)=1+j*i*(i+1)/2
 *            anti-diagonally
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   p_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/p_u128.h"

/* Auxiliary function: triangular number t(m) = m*(m+1)/2 (unsigned __int128) */
static inline unsigned __int128 tri_u128(idx_t m) {
    return (unsigned __int128)m * (m + 1) / 2;
}

seq_t a(idx_t n) {
    /* Find smallest m such that P(m) > n. */
    idx_t low = 0, high = 1;
    unsigned __int128 n128 = n;
    while (p_u128(high) <= n128)
        high <<= 1;            /* double until we exceed n */

    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (p_u128(mid) <= n128)
            low = mid;
        else
            high = mid;
    }
    idx_t m = high;                 /* m satisfies m(m+1)/2 <= n < (m+1)(m+2)/2 */
    unsigned __int128 prev_count = tri_u128(m);   /* S(m-1) = m(m+1)/2 */
    idx_t row = (idx_t)(n128 - prev_count);       /* n in formula */
    idx_t col = m - row;                           /* k in formula */

    /* Compute the value: 1 + col * row*(row+1)/2 */
    unsigned __int128 tri_row = tri_u128(row);
    unsigned __int128 val128 = (unsigned __int128)col * tri_row + 1;
    return (seq_t)val128;
}
