/*
 * A320473  First differences of A320472.
 * https://oeis.org/A320473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A320472(n+1) - A320472(n), where A320472(n) =
 *            round(x(n)), y(n) = y(n-1) + sin(x(n-1)), x(n) = x(n-1) + y(n),
 *            x(0)=y(0)=1
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Validated: partial, pass, 113 terms through n = 112 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    double x = 1.0;
    double y = 1.0;
    long long prev = llround(x);
    long long cur = prev;

    for (idx_t i = 0; i <= n; ++i) {
        y += sin(x);
        x += y;
        prev = cur;
        cur = llround(x);
    }
    return (seq_t)(cur - prev);
}
