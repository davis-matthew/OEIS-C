/*
 * A278490  Leaves in the tree defined by edge relation A255131(child) =
 *          parent, the least squares beanstalk.
 * https://oeis.org/A278490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m >= 1 such that no k has k - A002828(k) = m,
 *            i.e. A002828(m+j) != j for j = 1..4; found by a linear search
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 8972 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A278490_isq(uint64_t x)
{
    uint64_t r = 0, b = (uint64_t)1 << 31;
    while (b) {
        uint64_t t = r | b;
        if (t * t <= x) r = t;
        b >>= 1;
    }
    return r;
}

static int A278490_c(uint64_t m)
{
    uint64_t r, t, p;
    if (m == 0) return 0;
    r = A278490_isq(m);
    if (r * r == m) return 1;
    t = m;
    while ((t & 1) == 0) t >>= 1;
    bool two = true;
    for (p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; e++; }
            if ((p & 3) == 3 && (e & 1)) { two = false; break; }
        }
    }
    if (two && t > 1 && (t & 3) == 3) two = false;
    if (two) return 2;
    t = m;
    while ((t & 3) == 0) t >>= 2;
    if ((t & 7) == 7) return 4;
    return 3;
}

seq_t a(idx_t n)
{
    uint64_t m = 0, cnt = 0;
    while (cnt < (uint64_t)n) {
        m++;
        if (A278490_c(m + 1) != 1 && A278490_c(m + 2) != 2 &&
            A278490_c(m + 3) != 3 && A278490_c(m + 4) != 4)
            cnt++;
    }
    return (seq_t)m;
}
