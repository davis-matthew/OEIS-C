/*
 * A324291  Product prime(1+((n-1) mod (p-1))), where p ranges over distinct
 *          prime divisors of n.
 * https://oeis.org/A324291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p|n, p prime} prime(1 + ((n-1) mod (p-1)))
 * Range:     u8 n <= 186, u16 n <= 5116, u32 n <= 9601, u64 n <= 9601,
 *            u128 n <= 9601, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* factorization of n into distinct prime factors */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t product = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p     = primes[i];
        idx_t     k    = ((n - 1) % (p - 1)) + 1; /* 1‑based index */
        seq_t     pk   = (seq_t)get_prime(k);
        product *= pk;
    }
    return product;
}
