/*
 * A384547  Consecutive states of the linear congruential pseudo-random number
 *          generator 1749966429*s mod 2^32 when started at s=1.
 * https://oeis.org/A384547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = 1749966429 * a(n-1) mod 2^32 for n > 1
 *            Equivalent closed form: a(n) = 1749966429^(n-1) mod 2^32 This
 *            implementation uses binary exponentiation with 64‑bit
 *            intermediate arithmetic, which works for any seq_t width that
 *            can represent 32‑bit values
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Multiplier and modulus as 32‑bit values, cast to seq_t. */
    const seq_t mult = (seq_t)1749966429U;
    const seq_t M = (seq_t)0x100000000ULL;  /* 2^32 */

    /* Result starts at 1; exponent is n-1. */
    seq_t result = 1;
    uint64_t exp = n - 1;      /* n ≥ 1 by definition */
    seq_t base = mult;

    while (exp)
    {
        if (exp & 1ULL)
            result = (seq_t)(((uint64_t)result * (uint64_t)base) % M);

        /* Square the base for the next bit. */
        base = (seq_t)(((uint64_t)base * (uint64_t)base) % M);

        exp >>= 1;
    }

    return result;
}
