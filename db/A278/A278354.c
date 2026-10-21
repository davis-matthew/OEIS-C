/*
 * A278354  Number of neighbors of each new term in a square spiral.
 * https://oeis.org/A278354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 if n>=4 is in A002620 (square or n = m(m+1)); * a(n) =
 *            2 if n>=2 is in A033638 (n-1 is square or n-1 = m(m+1)); *
 *            otherwise a(n) = 4 if n>2, * with a(1)=0, a(2)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Special cases for offset */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* a(n) = 3 if n>=4 and n is a square or n = m(m+1) */
    if (n >= 4) {
        /* check perfect square */
        idx_t m = _floor_isqrt_u64(n);
        if (m * m == n) return (seq_t)3;

        /* check m(m+1) form */
        idx_t d = 1 + 4 * n;
        idx_t sqrt_d = _floor_isqrt_u64(d);
        if (sqrt_d * sqrt_d == d && (sqrt_d - 1) % 2 == 0) return (seq_t)3;
    }

    /* a(n) = 2 if n-1 is a square or n-1 = m(m+1) (n>2) */
    idx_t s = n - 1;
    /* check perfect square for s (m >= 2 to avoid n=3 case not counted yet) */
    if (s >= 4) {
        idx_t m = _floor_isqrt_u64(s);
        if (m * m == s && m >= 2) return (seq_t)2;
    }
    /* check m(m+1) form for s */
    idx_t d = 1 + 4 * s;
    idx_t sqrt_d = _floor_isqrt_u64(d);
    if (sqrt_d * sqrt_d == d && (sqrt_d - 1) % 2 == 0) return (seq_t)2;

    /* Default for n > 2 */
    return (seq_t)4;
}
