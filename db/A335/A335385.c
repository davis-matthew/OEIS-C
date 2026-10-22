/*
 * A335385  The number of tri-unitary divisors of n.
 * https://oeis.org/A335385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative, a(p^e)=4 if e=3 or e=6, otherwise 2, a(1)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses factor_u64 to obtain the prime factorisation. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    /* Allocate enough space for prime factors of a 64‑bit number. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        if (e == 3 || e == 6)
            result *= (seq_t)4;
        else
            result *= (seq_t)2;
    }
    return result;
}
