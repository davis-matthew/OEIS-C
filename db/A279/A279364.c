/*
 * A279364  Sum of 5th powers of proper divisors of n.
 * https://oeis.org/A279364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum of 5th powers of proper divisors of n
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 167, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Safe multiplication: use 128‑bit arithmetic for native integer types,
   otherwise rely on the type's operator*. */
#define MUL(a,b)                                                                           \
    (sizeof(seq_t)<=8 ? (seq_t)(((__uint128_t)(a)) * (__uint128_t)(b)) : (seq_t)(a)*(b))

seq_t a(idx_t n)
{
    /*  a(1) = 0  (no proper divisors) */
    if (n == 1) return 0;

    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t sigma5 = 1;

    for (size_t k = 0; k < cnt; ++k)
    {
        seq_t p = (seq_t)primes[k];
        uint64_t e = exps[k];

        /* Compute p^5 */
        seq_t p5 = 1;
        for (int i = 0; i < 5; ++i)
            p5 = MUL(p5, p);

        /* Sum of the geometric series 1 + p5 + p5^2 + ... + p5^e */
        seq_t term = 1;
        seq_t pow = 1;    /* p5^0 = 1 */
        for (uint64_t i = 1; i <= e; ++i)
        {
            pow = MUL(pow, p5);  /* pow = p5^i */
            term += pow;
        }

        sigma5 = MUL(sigma5, term);
    }

    /* Subtract n^5 to discard the divisor n itself */
    seq_t n5 = 1;
    for (int i = 0; i < 5; ++i)
        n5 = MUL(n5, (seq_t)n);

    return sigma5 - n5;
}
