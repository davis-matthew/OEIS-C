/*
 * A400267  The fourth least significant digit of n' [the arithmetic
 *          derivative of n, A003415] in the primorial base, A049345.
 * https://oeis.org/A400267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((n' mod 210) / 30) where n' is the arithmetic
 *            derivative of n. The value is the 4th least significant digit of
 *            n' in primorial base. The implementation computes n' mod 210
 *            from the prime factorization of n, then extracts the 30‑digit
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    /* Zero and one have derivative zero. */
    if (n <= 1)
        return (seq_t)0;

    const uint32_t MOD = 210U;   /* 2*3*5*7 */
    const uint32_t DIV = 30U;    /* 2*3*5 */

    /* Compute n' mod 210 using the factorisation of n. */
    uint64_t orig = (uint64_t)n;      /* original n used for n/p */
    uint64_t x = orig;                /* copy for factorisation */
    uint32_t der = 0;                 /* accumulate derivative mod 210 */

    /* Handle factor 2 first for speed. */
    if ((x & 1ULL) == 0) {
        uint32_t exp = 0;
        do {
            x >>= 1;
            exp++;
        } while ((x & 1ULL) == 0);
        uint32_t term = (uint32_t)((orig / 2ULL) % MOD);
        term = (uint32_t)( (term * (exp % MOD)) % MOD );
        der += term;
        if (der >= MOD) der -= MOD;
    }

    /* Trial division for odd primes. */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            uint32_t exp = 0;
            while (x % p == 0) {
                x /= p;
                exp++;
            }
            uint32_t term = (uint32_t)((orig / p) % MOD);
            term = (uint32_t)( (term * (exp % MOD)) % MOD );
            der += term;
            if (der >= MOD) der -= MOD;
        }
    }

    /* If a prime factor remains. */
    if (x > 1) {
        /* Remaining prime has exponent 1. */
        uint32_t term = (uint32_t)((orig / x) % MOD);
        der += term;
        if (der >= MOD) der -= MOD;
    }

    /* The required digit is the quotient of der mod 210 by 30. */
    uint32_t digit = der / DIV;
    return (seq_t)digit;
}
