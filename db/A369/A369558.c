/*
 * A369558  a(n) is the minimum value of k > 0 such that A075860(n) =
 *          A075860(n+k) with n > 1.
 * https://oeis.org/A369558
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k>0 : A075860(n) = A075860(n+k)}, found by forward
 *            search; A075860(n) = fixed point (prime) of x -> sum of distinct
 *            prime factors of x
 * Range:     u8 n <= 28, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 75 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t A075860_sdpf(uint64_t x) {
    uint64_t sum = 0;
    if ((x & 1ULL) == 0) {
        sum += 2;
        while ((x & 1ULL) == 0) x >>= 1;
    }
    for (uint64_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) {
            sum += d;
            while (x % d == 0) x /= d;
        }
    }
    if (x > 1) sum += x;
    return sum;
}

static bool A075860_isp(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

static uint64_t A075860_f(uint64_t n) {
    uint64_t cur = n;
    while (true) {
        if (A075860_isp(cur)) return cur;
        cur = A075860_sdpf(cur);
    }
}

seq_t a(idx_t n) {
    uint64_t base = A075860_f((uint64_t)n);
    uint64_t m = (uint64_t)n + 1;
    while (A075860_f(m) != base) m++;
    return (seq_t)(m - (uint64_t)n);
}
