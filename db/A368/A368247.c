/*
 * A368247  The number of cubefree divisors of the cubefull part of n
 *          (A360540).
 * https://oeis.org/A368247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n} (3 if e ≥ 3 else 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(p^e) = 1 for e ≤ 2, and 3 for e ≥ 3.              */

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 1 is the neutral element for multiplication. */
    if (n <= 1)
        return (seq_t)1;

    /* Temporary storage for factorisation.  A 64‑bit integer
       can have at most 64 prime factors (more than enough).  */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;

    /* Factorise n.  This helper will fill primes[]/exps[] and
       set pcnt to the number of distinct primes. */
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Count how many prime exponents are ≥ 3. */
    size_t cnt_ge3 = 0;
    for (size_t i = 0; i < pcnt; ++i)
        if (exps[i] >= 3)
            ++cnt_ge3;

    /* The result is 3 ^ cnt_ge3.  seq_pow works on seq_t values
       so we cast the base 3 to seq_t. */
    return seq_pow((seq_t)3, cnt_ge3);
}
