/*
 * A242357  Crescendo trapezoidal.
 * https://oeis.org/A242357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(k)=m for A104249(m-1)<=k<=A005448(m); in the gap
 *            A005448(m)+1..A104249(m)-1 (2m-1 terms) a runs
 *            m-1,...,1,1,2,...,m
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 102 terms through n = 859 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = 1;
    /* find largest m with start s_m = (3(m-1)^2+(m-1)+2)/2 <= n */
    for (;;) {
        idx_t t = m; /* s_{m+1} = A104249(m) */
        idx_t s_next = (3 * t * t + t + 2) / 2;
        if (s_next <= n) m++;
        else break;
    }
    idx_t e = 3 * (m * (m - 1) / 2) + 1; /* A005448(m) */
    if (n <= e) return (seq_t)m;
    idx_t d = n - e;
    if (d <= m - 1) return (seq_t)(m - d);
    return (seq_t)(d - (m - 1));
}
