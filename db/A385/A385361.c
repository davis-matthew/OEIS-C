/*
 * A385361  Consecutive states of the linear congruential pseudo-random number
 *          generator (2661*s + 36979) mod 175000 when started at s=1.
 * https://oeis.org/A385361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2661 * a(n-1) + 36979) mod 175000, with a(1) = 1. The
 *            recurrence is computed by fast exponentiation
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 52 terms through n = 9780 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* just in case */

static inline void lcg_pow_sum(idx_t k, seq_t *pow, seq_t *sum)
{
    const seq_t MOD = 175000;          /* modulus */
    const seq_t MUL = 2661;           /* multiplier */
    const seq_t INC = 36979;          /* increment */

    if (k == 0) {
        *pow = 1;
        *sum = 0;
        return;
    }

    if (k & 1) {                      /* k is odd: compute for k-1 and extend */
        seq_t p_prev, s_prev;
        lcg_pow_sum(k - 1, &p_prev, &s_prev);
        *pow = (p_prev * MUL) % MOD;
        *sum = (s_prev + (p_prev * INC) % MOD) % MOD;
    } else {                          /* k is even: split in half */
        seq_t p_half, s_half;
        lcg_pow_sum(k >> 1, &p_half, &s_half);
        *pow = (p_half * p_half) % MOD;
        seq_t mul = (p_half * s_half) % MOD;
        *sum = (s_half + mul) % MOD;
    }
}

seq_t a(idx_t n)
{
    const seq_t MOD = 175000;
    if (n < 1) return 0;          /* undefined for n<1; return 0 as safe fallback */
    if (n == 1) return 1;         /* first term */
    idx_t k = n - 1;               /* number of steps from initial state */
    seq_t pow, sum;
    lcg_pow_sum(k, &pow, &sum);
    seq_t result = (pow + sum) % MOD;   /* since a(1)=1, pow*1 = pow */
    return result;
}
