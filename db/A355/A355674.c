/*
 * A355674  Decimal expansion of 1351/780.
 * https://oeis.org/A355674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor( (1351 * 10^(n-1)) / 780 ) mod 10
 * Range:     u8 none, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Computes the nth decimal digit of 1351/780, with a(1) the integer part). */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n<1, but return 0 to be safe */

    const seq_t N   = 1351;        /* numerator */
    const seq_t D   = 780;         /* denominator */
    const seq_t MOD = 10 * D;      /* 7800, modulus */

    /* Compute 10^(n-1) mod MOD using binary exponentiation */
    idx_t exp = (n >= 1) ? (n - 1) : 0;
    seq_t base   = 10 % MOD;
    seq_t result = 1 % MOD;

    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    /* X = (N * 10^(n-1)) mod MOD */
    seq_t X = (result * (N % MOD)) % MOD;

    /* The quotient modulo 10 is floor(X / D) */
    seq_t digit = X / D;

    return digit;
}
