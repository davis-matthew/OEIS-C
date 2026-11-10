/*
 * A372352  The difference between n and the largest term of A086893 <= n.
 * https://oeis.org/A372352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - max(A086893 intersect [1..n]); A086893: a(1)=1,
 *            a(2)=3, a(k+2)=4*a(k)+1
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t cur = 1, nxt = 3, best = 0;
    while (cur <= N) {
        best = cur;
        uint64_t nw;
        if (cur > (UINT64_MAX - 1) / 4) nw = UINT64_MAX;
        else nw = 4 * cur + 1;
        cur = nxt;
        nxt = nw;
        if (best == UINT64_MAX) break;
    }
    return (seq_t)(N - best);
}
