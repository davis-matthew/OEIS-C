/*
 * A372772  a(n) is the number of divisors d of n such that d^n mod n = k,
 *          where k is also a divisor of n.
 * https://oeis.org/A372772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of divisors d of n such that d^n mod n is a divisor
 *            of n
 * Range:     u8 n <= 9195, u16 n <= 9195, u32 n <= 9195, u64 n <= 9195,
 *            u128 n <= 9195, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* fast modular exponentiation using uint64_t */
static inline uint64_t powmod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 0) return 0;
    base %= mod;
    uint64_t result = 1 % mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n = 0, but safe */
    uint64_t n64 = (uint64_t)n;
    seq_t count = 0;

    for (idx_t i = 1; (uint64_t)i * (uint64_t)i <= n64; ++i) {
        if (n64 % i == 0) {
            uint64_t d1 = i;
            uint64_t d2 = n64 / i;

            uint64_t r1 = powmod_u64(d1, n64, n64);
            if (r1 != 0 && n64 % r1 == 0) ++count;

            if (d1 != d2) {
                uint64_t r2 = powmod_u64(d2, n64, n64);
                if (r2 != 0 && n64 % r2 == 0) ++count;
            }
        }
    }

    return count;
}
