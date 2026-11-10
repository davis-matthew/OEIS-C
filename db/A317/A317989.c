/*
 * A317989  Number of genera of the real quadratic field with discriminant
 *          A003658(n), n >= 2.
 * https://oeis.org/A317989
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(omega(D)-1), D = A003658(n) (n-th fundamental
 *            discriminant of a real quadratic field); found by search,
 *            matches listed data
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9831 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A317989_sqfree(uint64_t n) {
    uint64_t i;
    for (i = 2; i <= n / i; i++) {
        if (n % (i * i) == 0) return false;
    }
    return true;
}

static bool A317989_fund(uint64_t k) {
    uint64_t r = k % 4;
    if (r == 1) return A317989_sqfree(k);
    if (r == 0) {
        uint64_t d = k / 4;
        uint64_t m = d % 4;
        if ((m == 2 || m == 3) && A317989_sqfree(d)) return true;
    }
    return false;
}

static int A317989_omega(uint64_t n) {
    int w = 0;
    uint64_t p;
    for (p = 2; p <= n / p; p++) {
        if (n % p == 0) {
            w++;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) w++;
    return w;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 0;
    int w;
    while (count < n) {
        k++;
        if (A317989_fund(k)) count++;
    }
    w = A317989_omega(k);
    if (w < 1) return (seq_t)1;
    return ((seq_t)1) << (w - 1);
}
