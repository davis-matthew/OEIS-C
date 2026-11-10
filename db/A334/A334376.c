/*
 * A334376  a(n) is the sum of digits of the n-th Moran number (A001101).
 * https://oeis.org/A334376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007953(A001101(n)); A001101(n) found by search: k such
 *            that k/digitsum(k) is prime
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A334376_is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

static uint64_t A334376_dsum(uint64_t k) {
    uint64_t s = 0;
    while (k > 0) {
        s += k % 10;
        k /= 10;
    }
    return s;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 0;
    uint64_t ds = 0;
    while (count < n) {
        k++;
        ds = A334376_dsum(k);
        if (ds > 0 && k % ds == 0 && A334376_is_prime(k / ds)) {
            count++;
        }
    }
    return (seq_t)ds;
}
