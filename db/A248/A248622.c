/*
 * A248622  Decimal expansion of 2^-50.
 * https://oeis.org/A248622
 *
 * Offset:    -15    (a(-15) is the first term)
 * Keywords:  nonn, cons, fini
 * Formula:   a(n) = floor(10^(n+31) / 2^50) mod 10, for n ≥ -15
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the nth decimal digit of 2^-50 (offset -15) */
#include <stdint.h>

static inline uint64_t modpow10(uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    uint64_t base   = 10 % mod;
    while (exp) {
        if (exp & 1U) {
            __uint128_t t = ( __uint128_t) result * base;
            result = (uint64_t)(t % mod);
        }
        __uint128_t t = ( __uint128_t) base * base;
        base = (uint64_t)(t % mod);
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Only defined for n >= -15; behaviour below offset is irrelevant */
    if (n < -15) return 0;          /* arbitrary fall‑back value */

    uint64_t exp = (uint64_t)(n + 31);          /* exponent n+31 ≥ 16 */
    uint64_t q   = (uint64_t)1 << 50;           /* 2^50  */
    uint64_t mod = q * 10ULL;                   /* 10 * 2^50  */

    uint64_t s   = modpow10(exp, mod);          /* 10^exp mod 10*q */
    uint64_t r   = s % q;                       /* remainder modulo q */
    uint64_t digit = (s - r) / q;               /* (10^exp / q) mod 10 */

    return (seq_t)digit;
}
