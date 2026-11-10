/*
 * A348058  a(n) = Min {k > n : A235137(k) == n (mod k)}, or -1 if no such
 *          minimum exists.
 * https://oeis.org/A348058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min {k > n : A235137(k) == n (mod k)}; search over k,
 *            using A235137(k) mod p^e == (k/p^e)*phi(p^e) mod p^e for each
 *            prime power p^e || k
 * Range:     u8 n <= 36, u16 n <= 85, u32 n <= 672, u64 n <= 672,
 *            u128 n <= 672, big any n
 * Validated: standard, pass, 77 terms through n = 622 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A348058_ok(uint64_t k, uint64_t n)
{
    uint64_t r = k;
    uint64_t p;
    for (p = 2; p * p <= r; p += (p == 2 ? 1 : 2)) {
        if (r % p == 0) {
            uint64_t q = 1;
            while (r % p == 0) { r /= p; q *= p; }
            uint64_t phi = q / p * (p - 1);
            uint64_t m = k / q;
            if ((m * phi) % q != n % q) return false;
        }
    }
    if (r > 1) {
        uint64_t q = r;
        uint64_t phi = q - 1;
        uint64_t m = k / q;
        if ((m * phi) % q != n % q) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    uint64_t k;
    for (k = (uint64_t)n + 1;; ++k) {
        if (A348058_ok(k, (uint64_t)n))
            return (seq_t)k;
    }
}
