/*
 * A329376  Multiplicative with a(p^e) = p when e = 2, otherwise a(p^e) = 1.
 * https://oeis.org/A329376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of distinct primes p such that the exponent of p
 *            in n is exactly 2 (The function is multiplicative with a(p^e)=p
 *            when e=2, otherwise a(p^e)=1)
 * Range:     u8 n <= 9983, u16 n <= 9983, u32 n <= 9983, u64 n <= 9983,
 *            u128 n <= 9983, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides void factor_u64(uint64_t, uint64_t *, uint64_t *, size_t *) */

seq_t a(idx_t n)
{
    if (n == 0)        /* the offset starts at 1; behaviour for n==0 is irrelevant, but guard it */
        return (seq_t)0;

    /* allocate temporary storage for prime factorization */
    /* 64 is enough for 64‑bit n – the number of distinct prime factors is at most 16,
       but we use a generous limit. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] == 2)
            prod *= (seq_t)primes[i];
    }
    return prod;
}
