/*
 * A316111  a(n) is the smallest k > 1 such that gcd(k, n^k - n) = 1, for n >
 *          1.
 * https://oeis.org/A316111
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 1 such that gcd(k, n^k - n) = 1
 * Range:     u8 n <= 54, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* modular multiplication using 128‑bit intermediate for safety */
static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* modular exponentiation: base^exp mod mod, mod ≥ 2 */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mul_mod_uint64(result, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;               /* n fits in 64 bits for the limits */
    if (nn <= 1) return 0;                   /* undefined for n <= 1 */
    for (uint64_t k = 2;; ++k) {
        /* k must be coprime to n otherwise gcd cannot be 1 */
        if (gcd((seq_t)k, (seq_t)nn) != 1) continue;
        uint64_t powmod = mod_pow_uint64(nn % k, k, k);
        uint64_t modn   = nn % k;
        uint64_t diff   = (powmod >= modn) ? powmod - modn : powmod + k - modn;
        if (gcd((seq_t)k, (seq_t)diff) == 1) return (seq_t)k;
    }
}
