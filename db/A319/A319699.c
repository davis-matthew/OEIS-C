/*
 * A319699  a(n) = A001065(A252463(n)).
 * https://oeis.org/A319699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001065(A252463(n)); A252463(1)=1, A252463(2n)=n,
 *            A252463(2n+1)=prod prev_prime(p)^e over p^e||2n+1;
 *            A001065(m)=sigma(m)-m
 * Range:     u8 n <= 287, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 8538 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A319699_isp(uint64_t x) {
    if (x < 2) return false;
    if (x < 4) return true;
    if ((x & 1u) == 0u) return false;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A319699_prevp(uint64_t n) {
    uint64_t p = n - 1;
    while (!A319699_isp(p)) --p;
    return p;
}

static uint64_t A319699_A252463(uint64_t n) {
    if (n == 1) return 1;
    if ((n & 1u) == 0u) return n >> 1;
    uint64_t m = n, result = 1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t base = A319699_prevp(p);
            while (m % p == 0) {
                m /= p;
                result *= base;
            }
        }
    }
    if (m > 1) result *= A319699_prevp(m);
    return result;
}

seq_t a(idx_t n) {
    uint64_t m = A319699_A252463((uint64_t)n);
    seq_t sum = 0;
    for (uint64_t i = 1; i <= m / i; i++) {
        if (m % i == 0) {
            if (i < m) sum += (seq_t)i;
            uint64_t c = m / i;
            if (c < m && c != i) sum += (seq_t)c;
        }
    }
    return sum;
}
