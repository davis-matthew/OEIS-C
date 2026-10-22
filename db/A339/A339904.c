/*
 * A339904  The odd part of {Euler totient function phi applied to the prime
 *          shifted n}: a(n) = A000265(A000010(A003961(n))).
 * https://oeis.org/A339904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e∥n} odd_part(next_prime(p)-1) * next_prime(p)^{e-1}
 *            (i.e. the multiplicative definition of A339904)
 * Range:     u8 n <= 127, u16 n <= 4095, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   factor_u64, next_prime, odd_part, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/odd_part.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n = 0 we return 0. */
    if (n < 1) return (seq_t)0;

    /* Factorise n into prime powers.  The maximum number of distinct
       prime factors of a 64‑bit integer is below 64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Next prime after p */
        uint64_t q = next_prime(p);

        /* odd part of q‑1 */
        seq_t odd = odd_part((seq_t)(q - 1));

        /* q^(e‑1); for e = 1 we use 1 */
        seq_t qpow = (e > 1) ? pow_uint(q, (idx_t)(e - 1)) : (seq_t)1;

        /* term for this prime power */
        seq_t term = seq_mul(odd, qpow);

        /* accumulate product */
        result = seq_mul(result, term);
    }

    return result;
}
