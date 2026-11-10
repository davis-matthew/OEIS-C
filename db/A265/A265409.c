/*
 * A265409  a(n) = index to the nearest inner neighbor in Ulam-style
 *          square-spirals using zero-based indexing.
 * https://oeis.org/A265409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n <= 7; for n >= 8, a(n) = a(n-1) +
 *            (1-A240025(n))*(1-A240025(n-1)), where A240025 is the indicator
 *            of quarter-squares floor(k^2/4)
 * Range:     u8 n <= 322, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9706 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 7) return (seq_t)0;
    idx_t k = 6;
    idx_t nq = (k * k) / 4;   /* next quarter-square >= m, starting at 9 */
    int prev = 0;             /* indicator for m-1 = 7: not a quarter-square */
    idx_t cnt = 0;
    for (idx_t m = 8; m <= n; ++m) {
        int cur = (m == nq);
        if (cur) {
            ++k;
            nq = (k * k) / 4;
        }
        if (!cur && !prev) ++cnt;
        prev = cur;
    }
    return (seq_t)cnt;
}
