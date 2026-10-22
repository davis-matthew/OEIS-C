/*
 * A386310  Number of divisors d of n such that 2*d^d == 0 (mod n).
 * https://oeis.org/A386310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that 2*d^d == 0 (mod n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static inline uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    if (mod == 0) return 0;
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t count = (seq_t)0;
    /* iterate over all divisors of n */
    idx_t i = 1;
    for (; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor i */
            uint64_t d = i;
            uint64_t mod = n;
            uint64_t pow = pow_mod_uint64(d, d, mod);
            if ((pow * 2ULL) % mod == 0) count += (seq_t)1;
            /* divisor n/i, avoid double counting when i == n/i */
            if (i != n / i) {
                d = n / i;
                pow = pow_mod_uint64(d, d, mod);
                if ((pow * 2ULL) % mod == 0) count += (seq_t)1;
            }
        }
    }
    return count;
}
