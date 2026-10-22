/*
 * A384643  Consecutive states of the linear congruential pseudo-random number
 *          generator for Simula on the UNIVAC when started at 1.
 * https://oeis.org/A384643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^15)^(n-1) mod 2^35, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Modulus is 2^35; we keep the lower 35 bits. */
static const uint64_t MODMASK64 = ((uint64_t)1 << 35) - 1;
/* Multiplier M = 5^15 */
static const uint64_t MUL_CONST64 = 30517578125ULL;   /* 5^15 */

/* Multiply two 64‑bit numbers modulo 2^35 using 128‑bit arithmetic. */
static inline uint64_t mul_mod_2k(uint64_t a, uint64_t b)
{
    return (uint64_t)(((__uint128_t)a * b) & MODMASK64);
}

/* Power modulo 2^35 via binary exponentiation. */
static inline uint64_t pow_mod_2k(uint64_t base, idx_t exp)
{
    uint64_t result = 1;
    uint64_t cur = base & MODMASK64;
    while (exp)
    {
        if (exp & 1) result = mul_mod_2k(result, cur);
        exp >>= 1;
        if (exp) cur = mul_mod_2k(cur, cur);
    }
    return result;
}

/* Sequence term generator. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but safe */
    if (n == 1) return 1;
    uint64_t res64 = pow_mod_2k(MUL_CONST64, n - 1);
    return (seq_t)res64;
}
