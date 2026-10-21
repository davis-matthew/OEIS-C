/*
 * A295878  Multiplicative with a(p^(2e)) = 1, a(p^(2e-1)) = prime(e).
 * https://oeis.org/A295878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of the ((e+1)/2)-th prime for every prime power
 *            p^e dividing n with e odd. Thus a(p^(2e)) = 1 and a(p^(2e-1)) =
 *            prime(e)
 * Range:     u8 n <= 8554, u16 n <= 8554, u32 n <= 8554, u64 n <= 8554,
 *            u128 n <= 8554, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Functions supplied by the utilities. */
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"

/* Multiplicative sequence A295878. */
seq_t a(idx_t n)
{
    /* a(1) is defined as 1. */
    if (n == 1)
        return (seq_t)1;

    /* Arrays large enough for any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   count;

    /* Factor n and obtain its prime exponents. */
    factor_u64((uint64_t)n, primes, exps, &count);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < count; ++i)
    {
        uint64_t e = exps[i];

        /* Only odd exponents contribute. */
        if ((e & 1) != 0)
        {
            /* k = (e+1)/2 is the index of the prime to use. */
            idx_t k = (idx_t)((e + 1) / 2);
            seq_t p = (seq_t)get_prime(k);
            result *= p;
        }
    }

    return result;
}
