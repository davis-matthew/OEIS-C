/*
 * A349131  a(n) = Sum_{d|n} phi(d) * A003958(n/d), where A003958 is fully
 *          multiplicative with a(p) = (p-1), and phi is Euler totient
 *          function.
 * https://oeis.org/A349131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} phi(d) * A003958(n/d)
 * Range:     u8 n <= 84, u16 n <= 7314, u32 n <= 9358, u64 n <= 9358,
 *            u128 n <= 9358, big any n
 * Helpers:   factor_u64, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"  /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/pow_uint.h"   /* pow_uint(uint64_t base, idx_t exp) */
#include "../utils/seq_mul.h"    /* seq_mul(seq_t a, seq_t b) -- optional, used via × operator */

/* Computes the sequence A349131: a(1)=1, multiplicative with a(p^e) =
   (p-1)*p^e - (p-2)*(p-1)^e.  Works for any seq_t and idx_t. */
seq_t a(idx_t n)
{
    /* Handles n>=1; for n==0 the value is irrelevant (offset is 1). */
    seq_t result = 1;

    /* Factor n.  An array of 64 slots suffices for 64‑bit n. */
    size_t cnt = 0;
    uint64_t primes[64], exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t p_pow     = pow_uint(p,     e);   /* p^e   */
        seq_t pm1_pow   = pow_uint(p-1,   e);   /* (p-1)^e */

        seq_t part1 = (seq_t)(p-1) * p_pow;      /* (p-1)*p^e   */
        seq_t part2 = (seq_t)(p-2) * pm1_pow;    /* (p-2)*(p-1)^e */

        seq_t local = part1 - part2;             /* local contribution  */
        result *= local;                         /* multiply into product  */
    }

    return result;
}
