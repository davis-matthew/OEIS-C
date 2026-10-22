/*
 * A353494  a(n) = 1 if the arithmetic derivative of n is a multiple of 4,
 *          otherwise 0.
 * https://oeis.org/A353494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the arithmetic derivative d(n) is a multiple of 4,
 *            otherwise 0. d(n) is computed via the prime‐factorisation
 *            formula d(n) = Σ e_i * n / p_i, where n = Π p_i^e_i
 * Range:     u8 n <= 450, u16 n <= 8963, u32 n <= 8963, u64 n <= 8963,
 *            u128 n <= 8963, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Must include the helper that provides the prime factorisation routine. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr,
                                   uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n)
{
    /* d(0) = 0 and d(1) = 0, so both are multiples of 4. */
    if (n <= 1)
        return (seq_t)1;

    /* Factorise n.  An array of 64 elements is more than enough
       for n up to 2^64 − 1. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Sum the terms e_i * (n / p_i) modulo 4. */
    seq_t sum_mod = 0;
    const seq_t FOUR = 4;           /* Use a seq_t constant for consistency. */

    for (size_t i = 0; i < cnt; ++i)
    {
        /* n / p_i is exact because p_i divides n. */
        seq_t qp   = (seq_t)n / (seq_t)primes[i];

        /* (qp % 4) * (exps[i] % 4) mod 4   --  use bitwise &3 for mod4. */
        seq_t term = ((qp & 3) * (exps[i] & 3)) & 3;

        sum_mod = (sum_mod + term) & 3;
    }

    return (sum_mod == 0) ? (seq_t)1 : (seq_t)0;
}
