/*
 * A367203  Product of p^(v_p(n)-1) + p^(2*floor(v_p(n)/2)) over all prime
 *          divisors p of n with valuation v_p(n).
 * https://oeis.org/A367203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product_{p^e ‖ n} (p^(e-1) + p^(2*floor(e/2))), a(1)=1
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Computes the nth term of OEIS sequence A367203. */
seq_t a(idx_t n)
{
    /* n == 1 yields an empty product. */
    if (n == 1)
        return (seq_t)1;

    /* Temporary storage for the prime factorisation of n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    /* Factor n (provided by the framework). */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t    e = (idx_t)exps[i];          /* exponent of p in n */

        /* Compute p^(e-1). */
        seq_t term1 = pow_uint(p, e - 1);

        /* Compute p^(2*floor(e/2)) = p^(e - (e & 1)). */
        idx_t exp2 = e - (e & 1);
        seq_t term2 = pow_uint(p, exp2);

        result *= (term1 + term2);
    }
    return result;
}
