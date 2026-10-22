/*
 * A372999  a(n) = Sum_{k=1..n} tau( (n/gcd(k,n))^4 ).
 * https://oeis.org/A372999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} phi(d) * tau(d^4)
 * Range:     u8 n <= 11, u16 n <= 329, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* The multiplicative form used below follows a(p^e) = 1 + (p-1)*
   Σ_{k=1..e} (4k+1)p^(k-1) */
seq_t a(idx_t n)
{
    /* Factorise n; primes and exponents stored in small buffers */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        size_t  e = exps[i];

        seq_t ak  = 1;       /* a(p^e) accumulator */
        seq_t pow = 1;       /* p^(k-1), starts with 1 for k=1 */

        for (idx_t k = 1; k <= (idx_t)e; ++k) {
            seq_t term = ((seq_t)(4 * k + 1)) * pow;  /* (4k+1)*p^(k-1) */
            ak += term * (p - 1);
            pow *= p;                         /* next p^(k) for loop */
        }
        result *= ak;
    }

    return result;
}
