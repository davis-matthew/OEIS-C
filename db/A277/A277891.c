/*
 * A277891  a(n) = number of odd numbers encountered before reaching (n^2)-1
 *          when starting from k = ((n+1)^2)-1 and iterating map k -> k -
 *          A002828(k).
 * https://oeis.org/A277891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd k among the iterates of k -> k -
 *            A002828(k), starting at k=(n+1)^2-1 and stopping before k=n^2-1
 *            (direct iteration)
 * Range:     u8 n <= 716, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 9450 (2026-11-08)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

static int A277891_two_squares(uint64_t m) {
    uint64_t p;
    while (m % 2 == 0) m /= 2;
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (p % 4 == 3 && (e & 1)) return 0;
        }
    }
    if (m > 1 && m % 4 == 3) return 0;
    return 1;
}

static uint64_t A277891_A002828(uint64_t m) {
    uint64_t r;
    if (m == 0) return 0;
    r = isqrt_u64(m);
    if (r * r == m) return 1;
    {
        uint64_t t = m;
        while (t % 4 == 0) t /= 4;
        if (t % 8 == 7) return 4;
    }
    if (A277891_two_squares(m)) return 2;
    return 3;
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t k = (nn + 1) * (nn + 1) - 1;
    uint64_t target = nn * nn - 1;
    uint64_t count = 0;
    while (k > target) {
        if (k & 1) count++;
        k -= A277891_A002828(k);
    }
    return (seq_t)count;
}
