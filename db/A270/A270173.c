/*
 * A270173  Smallest number k such that A270172(k) = n.
 * https://oeis.org/A270173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0; for n>=1 the least k with A270172(k)=n has m=ceil(n/9)
 *            significant digits followed by a 0; the digits are chosen
 *            greedily, smallest first, from the strip-time recurrence t_p =
 *            t_{p-1} + ((-d_p - t_{p-1}) mod 10), t_{-1}=1, with t_{m-1}=n
 * Range:     u8 n <= 18, u16 n <= 36, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 43 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint64_t N = (uint64_t)n;
    uint64_t m = (N + 8) / 9;
    uint64_t tprev = 1;
    seq_t val = 0;

    for (uint64_t p = 0; p < m; ++p) {
        uint64_t rem = m - 1 - p;
        int start = (p == 0) ? 1 : 0;
        int chosen = -1;
        uint64_t tchosen = 0;
        for (int d = start; d <= 9; ++d) {
            uint64_t need = (uint64_t)((10 - d) % 10);
            uint64_t cur = tprev % 10;
            uint64_t r = (need + 10 - cur) % 10;
            uint64_t t = tprev + r;
            if (t > N)
                continue;
            if (N - t <= 9 * rem) {
                chosen = d;
                tchosen = t;
                break;
            }
        }
        if (chosen < 0)
            chosen = 0; /* should not happen */
        val = val * 10 + (seq_t)chosen;
        tprev = tchosen;
    }
    val = val * 10;
    return val;
}
