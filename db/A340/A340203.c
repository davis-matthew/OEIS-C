/*
 * A340203  Number of primes in A339399 among the values of A339399(k) for k =
 *          1..n.
 * https://oeis.org/A340203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} [c(k) is prime], c = A339399 (blocks
 *            m=2,3,...: s=1..floor(m/2) listing s, m-s); direct generation
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A340203_isp(uint64_t x) {
    if (x < 2) return 0;
    if (x < 4) return 1;
    if ((x & 1) == 0) return 0;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

seq_t a(idx_t n) {
    uint64_t idx = 0;
    seq_t count = 0;
    if (n == 0) return count;
    for (uint64_t m = 2; ; m++) {
        for (uint64_t s = 1; s <= m / 2; s++) {
            if (A340203_isp(s)) count++;
            idx++;
            if (idx >= n) return count;
            if (A340203_isp(m - s)) count++;
            idx++;
            if (idx >= n) return count;
        }
    }
}
