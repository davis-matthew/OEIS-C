/*
 * A277486  a(n) = number of integers one more than a prime encountered before
 *          reaching (n^2)-1 when starting from k = ((n+1)^2)-1 and iterating
 *          map k -> k - A002828(k).
 * https://oeis.org/A277486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k in the orbit of K -> K - A002828(K), starting
 *            at K = (n+1)^2-1 and stopping on reaching a value <= n^2-1 (that
 *            value excluded), with k-1 prime. Computed by direct iteration
 * Range:     u8 n <= 5252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 131 terms through n = 9119 (2026-11-08)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"
#include "../utils/is_prime_u64.h"

static int s2828_two_squares(uint64_t m) {
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
    if (s2828_two_squares(m)) return 2;
    while (m % 4 == 0) m /= 4;
    if (m % 8 == 7) return 4;
    return 3;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t lo = N * N - 1;
    uint64_t k = (N + 1) * (N + 1) - 1;
    uint64_t cnt = 0;
    while (k > lo) {
        if (k >= 3 && is_prime_u64(k - 1)) cnt++;
        k -= A002828_a(k);
    }
    return (seq_t)cnt;
}
