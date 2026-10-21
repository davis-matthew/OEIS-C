/*
 * A260636  a(n) = binomial(3n, n) mod n.
 * https://oeis.org/A260636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3n, n) mod n (offset 1)
 * Range:     u8 n <= 273, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* binomial(3,1)=3 ≡ 0 (mod 1) */

    /* Sieve primes up to 3n */
    idx_t limit = 3 * n;
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime)          /* Allocation failure – should not happen */
        return 0;

    for (idx_t i = 2; i <= limit; ++i)
        is_prime[i] = true;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!is_prime[p])
            continue;
        for (idx_t m = p * p; m <= limit; m += p)
            is_prime[m] = false;
    }

    /* Compute binomial(3n,n) mod n via prime exponents */
    seq_t result = 1 % (seq_t)n;

    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p])
            continue;

        /* Legendre exponent of p in (3n)!/(n!*(2n)!) */
        idx_t e = 0, m;

        m = limit;                  /* (3n)! */
        while (m) { m /= p; e += m; }
        m = n;                      /* subtract n! */
        while (m) { m /= p; e -= m; }
        m = 2 * n;                  /* subtract (2n)! */
        while (m) { m /= p; e -= m; }

        if (e == 0)
            continue;

        seq_t base = (seq_t)(p % (idx_t)n);
        seq_t pow_term = mod_pow(base, (uint64_t)e, (seq_t)n);
        result = (result * pow_term) % (seq_t)n;
    }

    free(is_prime);
    return result;
}
