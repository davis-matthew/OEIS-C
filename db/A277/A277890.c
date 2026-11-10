/*
 * A277890  Number of even numbers encountered before (n^2)-1 is reached when
 *          starting from k = ((n+1)^2)-1 and iterating map k -> k -
 *          A002828(k).
 * https://oeis.org/A277890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even k among the iterates of k -> k -
 *            A002828(k), starting at k = (n+1)^2-1 and stopping on reaching
 *            n^2-1 (end excluded); computed by direct iteration
 * Range:     u8 n <= 762, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 113 terms through n = 9457 (2026-11-08)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

static int A002828_two_squares(uint64_t m) {
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

static uint64_t A002828_a(uint64_t m) {
    uint64_t r;
    if (m == 0) return 0;
    r = isqrt_u64(m);
    if (r * r == m) return 1;
    if (A002828_two_squares(m)) return 2;
    while (m % 4 == 0) m /= 4;
    if (m % 8 == 7) return 4;
    return 3;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t k = (N + 1) * (N + 1) - 1;
    uint64_t target = N * N - 1;
    seq_t count = 0;
    while (k > target) {
        if ((k & 1) == 0) count++;
        k -= A002828_a(k);
    }
    return count;
}
