/*
 * A318475  Additive with a(p^e) = A000108(e).
 * https://oeis.org/A318475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ Catalan(e_i) over the prime power exponents e_i in the
 *            factorization of n (a(1)=0). This is the additive arithmetic
 *            function with a(p^e)=Catalan(e)
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 8959, u64 n <= 8959,
 *            u128 n <= 8959, big any n
 * Helpers:   catalan, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n <= 1u)                 /* a(1) = 0, and below offset is irrelevant */
        return (seq_t)0;

    /* Factor n into primes and exponents.  A 64‑bit integer has at most 16 distinct prime factors. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i)
        sum += catalan((idx_t)exps[i]);

    return sum;
}
