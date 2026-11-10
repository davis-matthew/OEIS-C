/*
 * A354095  a(n) = A354092(sigma(n)).
 * https://oeis.org/A354095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A354092(sigma(n)); multiplicative, a(p^e) =
 *            A354092(1+p+...+p^e)
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9235 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A354095_isprime(uint64_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    if (x % 3 == 0) return x == 3;
    for (uint64_t d = 5; d * d <= x; d += 6) {
        if (x % d == 0 || x % (d + 2) == 0) return false;
    }
    return true;
}

static uint64_t A354095_map_prime(uint64_t p) {
    if (p == 2) return 1;
    if (p % 3 != 2) return p;
    uint64_t q = p - 1;
    while (q > 2) {
        if (q % 3 == 2 && A354095_isprime(q)) return q;
        --q;
    }
    return 2;
}

/* A354092 applied to m (fully multiplicative prime shift) */
static seq_t A354095_shift(uint64_t m) {
    seq_t result = 1;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t base = A354095_map_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= (seq_t)base;
            }
        }
    }
    if (m > 1) result *= (seq_t)A354095_map_prime(m);
    return result;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    seq_t result = 1;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= p;
                term += pw;
            }
            result *= A354095_shift(term);
        }
    }
    if (m > 1) result *= A354095_shift(m + 1);
    return result;
}
