/*
 * A334195  a(1) = 0, then after the first differences of A064415.
 * https://oeis.org/A334195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 0; a(n) = A064415(n) - A064415(n-1) for n > 1, where
 *            A064415(n) = A003434(n) for even n and A003434(n)-1 for odd n,
 *            and A003434(n) = number of iterations of phi needed to reach 1
 * Range:     i8 n <= 65537, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 7501 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A334195_phi(uint64_t n) {
    uint64_t r = n;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            r -= r / p;
        }
    }
    if (n > 1) r -= r / n;
    return r;
}

static long long A334195_iter(uint64_t n) {
    long long c = 0;
    while (n > 1) {
        n = A334195_phi(n);
        ++c;
    }
    return c;
}

static long long A334195_A064415(uint64_t n) {
    if (n <= 1) return 0;
    long long it = A334195_iter(n);
    if (n & 1) it -= 1;
    return it;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    long long d = A334195_A064415((uint64_t)n) - A334195_A064415((uint64_t)n - 1);
    return (seq_t)d;
}
