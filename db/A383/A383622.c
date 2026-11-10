/*
 * A383622  First differences of A383621.
 * https://oeis.org/A383622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th smallest term (with repeats) of the multiset
 *            {A027649(i)*A007310(j) : i>=0, j>=1}; found by binary search on
 *            the counting function
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9773 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A383622_count(seq_t x)
{
    seq_t total = 0;
    seq_t A = 1;      /* A027649(i) */
    seq_t p2 = 1;     /* 2^i */
    while (A <= x) {
        seq_t m = x / A;
        seq_t r = m % 6;
        total += (m / 6) * 2 + (r >= 1 ? 1 : 0) + (r >= 5 ? 1 : 0);
        seq_t next = A * 3 + p2;
        if (next <= A) break;
        A = next;
        p2 = p2 * 2;
    }
    return total;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    seq_t lo = 1;
    seq_t hi = target * 3 + 6;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (A383622_count(mid) >= target)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}
