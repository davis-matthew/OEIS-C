/*
 * A384402  Consecutive states of the linear congruential pseudo-random number
 *          generator 39373*s mod (2^31-1) when started at s=1.
 * https://oeis.org/A384402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = 39373 * a(n‑1) mod (2^31‑1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Modulus: 2^31−1 = 2147483647 */
    const seq_t MOD = (seq_t)2147483647ULL;
    const seq_t MUL = (seq_t)39373U;

    if (n == 1) return (seq_t)1;      /* base value */
    if (n == 0) return (seq_t)0;      /* not used, safe placeholder */

    idx_t      exp    = n - 1;        /* exponent: MUL^(n-1) */
    seq_t      result = (seq_t)1;
    seq_t      base   = MUL;

    /* fast exponentiation modulo MOD */
    while (exp)
    {
        if (exp & 1ULL) result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}
