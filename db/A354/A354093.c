/*
 * A354093  a(n) = sigma(A354091(n)), where A354091 is fully multiplicative
 *          prime shift which replaces the primes of the form 3k+2 by the next
 *          larger such prime, while other primes stay as they are, and sigma
 *          is the sum of divisors function.
 * https://oeis.org/A354093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(A354091(n)) where A354091 is fully multiplicative:
 *            for a prime p, if p ≡ 2 (mod 3) → replace p by the next prime >
 *            p having the same residue (i.e., the next prime ≡ 2 (mod 3));
 *            otherwise keep p unchanged. For a composite, A354091(n) = ∏
 *            q_i^e_i (q_i = mapped prime). Then a(n) = σ(A35...
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/big_pow.h"

static seq_t shift_prime(uint64_t p)
{
    if (p % 3 != 2)      /* not of the form 3k+2 → no shift */
        return (seq_t)p;

    uint64_t q = next_prime(p + 1);          /* start just above p */
    while (q % 3 != 2)                       /* find next prime ≡2 (mod 3) */
        q = next_prime(q + 1);
    return (seq_t)q;
}

seq_t a(idx_t n)
{
    if (n == 1)                         /* 1 has no prime factors, σ(1)=1 */
        return (seq_t)1;

    /* arrays for factorization – enough for n up to 10^6 */
    static uint64_t primes[128];
    static uint64_t exps[128];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t q = shift_prime(p);                     /* mapped prime      */
        seq_t exp = (seq_t)(e + 1);                   /* exponent for power */
        seq_t power = big_pow(q, exp);                /* q^(e+1)           */
        seq_t term   = (power - (seq_t)1) / (q - (seq_t)1);
        sigma *= term;
    }
    return sigma;
}
