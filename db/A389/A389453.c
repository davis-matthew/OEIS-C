/*
 * A389453  Arithmetic derivative of the largest p-full divisor of n (a term
 *          of A054744 that divides n).
 * https://oeis.org/A389453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(A368333(n)) – arithmetic derivative of the
 *            largest p‑full divisor of n. For each prime p dividing n, a
 *            prime power p^e contributes to the p‑full divisor only if e ≥ p.
 *            The arithmetic derivative is then computed by the usual rules
 * Range:     u8 n <= 127, u16 n <= 8391, u32 n <= 8391, u64 n <= 8391,
 *            u128 n <= 8391, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Fast exponentiation: base^exp for seq_t, exp of type idx_t.  */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp != 0)
    {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n >= 1 by specification; for safety return 0 for n < 1 */
    if (n < 1)
        return 0;

    /* Factor n.  factor_u64 returns primes[] and exps[] with count cnt. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Construct the largest p‑full divisor d of n:
       include prime power p^e only if e >= p. */
    seq_t d = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (exps[i] >= primes[i])
        {
            seq_t pw = pow_seq((seq_t)primes[i], (idx_t)exps[i]);
            d *= pw;
        }
    }

    /* Compute arithmetic derivative of d:
       d' = Σ (d / p) * e_p over the primes that satisfy e_p >= p. */
    seq_t deriv = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (exps[i] >= primes[i])
        {
            seq_t div = d / (seq_t)primes[i];
            deriv += div * (seq_t)exps[i];
        }
    }

    return deriv;
}
