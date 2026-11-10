/*
 * A269267  Indices k such that A107435(k) != A268057(k).
 * https://oeis.org/A269267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k-th index m (in increasing order) with A107435(m) !=
 *            A268057(m); computed by testing each index m in turn. A107435
 *            counts Euclid steps on (r,c). A268057 counts iterations of c ->
 *            r mod c until 0 (r fixed). Here r is the row and c is the column
 *            of m in the triangle read by rows
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9464 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static void a269267_rowcol(idx_t m, idx_t *r, idx_t *c)
{
    idx_t lo = 1, hi = 1;
    while (hi * (hi + 1) / 2 < m)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) / 2;
        if (mid * (mid + 1) / 2 >= m)
            hi = mid;
        else
            lo = mid + 1;
    }
    *r = lo;
    *c = m - lo * (lo - 1) / 2;
}

static bool a269267_differs(idx_t m)
{
    idx_t r, c;
    a269267_rowcol(m, &r, &c);

    /* A107435: Euclidean algorithm steps */
    uint64_t x = r, y = c, s1 = 0;
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
        ++s1;
    }

    /* A268057: iterate k -> r mod k */
    uint64_t k = c, s2 = 0;
    while (k != 0) {
        k = (uint64_t)r % k;
        ++s2;
    }
    return s1 != s2;
}

seq_t a(idx_t n)
{
    static idx_t last_n = 0;
    static idx_t last_m = 0;
    idx_t cnt, m;

    if (last_n != 0 && n >= last_n) {
        cnt = last_n;
        m = last_m;
    } else {
        cnt = 0;
        m = 0;
    }
    while (cnt < n) {
        ++m;
        if (a269267_differs(m))
            ++cnt;
    }
    last_n = n;
    last_m = m;
    return (seq_t)m;
}
