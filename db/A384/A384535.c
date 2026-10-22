/*
 * A384535  Consecutive states of the linear congruential pseudo-random number
 *          generator 1566083941*s mod 2^32 when started at s=1.
 * https://oeis.org/A384535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1566083941 * a(n-1) mod 2^32, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply two 32‑bit values and keep only the low 32 bits. */
static inline seq_t mul_mod32(seq_t x, seq_t y)
{
    /* values are known to fit in 32 bits, so cast to 64‑bit for safety */
    return (seq_t)((((uint64_t)(unsigned long long)x) *
                    ((uint64_t)(unsigned long long)y)) & 0xffffffffULL);
}

/* Fast modular exponentiation with modulus 2^32. */
static seq_t mod_pow32(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result = mul_mod32(result, base);
        base = mul_mod32(base, base);
        exp >>= (idx_t)1;
    }
    return result;
}

/* Sequence function required by the problem. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n<1, but avoid UB */
    /* a(1) = 1 = 1566083941^(0) mod 2^32, so compute power n-1 */
    const seq_t multiplier = (seq_t)1566083941ULL;
    return mod_pow32(multiplier, n - (idx_t)1);
}
