/*
 * A281856  One fourth of the order of the abelian non-cyclic groups
 *          (Z/A033949(n)*Z)^x.
 * https://oeis.org/A281856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000010(A033949(n))/4, where A033949 = integers with no
 *            primitive root (found by search)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Returns true if m>1 is a power of a single odd prime. */
static bool A281856_odd_prime_power(uint64_t m) {
    if (m <= 1) return false;
    if ((m & 1ULL) == 0) return false;
    uint64_t p = m;
    for (uint64_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) { p = i; break; }
    }
    while (m % p == 0) m /= p;
    return m == 1;
}

static bool A281856_has_no_primitive_root(uint64_t c) {
    if (c <= 2 || c == 4) return false;
    if (A281856_odd_prime_power(c)) return false;
    if ((c & 3ULL) == 2 && A281856_odd_prime_power(c >> 1)) return false;
    return true;
}

static uint64_t A281856_totient(uint64_t m) {
    uint64_t result = m;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t c = 2;
    while (count < (uint64_t)n) {
        ++c;
        if (A281856_has_no_primitive_root(c)) ++count;
    }
    return (seq_t)(A281856_totient(c) / 4);
}
