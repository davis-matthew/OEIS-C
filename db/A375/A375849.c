/*
 * A375849  The maximum odd exponent in the prime factorization of n!.
 * https://oeis.org/A375849
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum odd exponent in the prime factorization of n!.
 *            a(n) = max{ e_p(n!) : 2 | e_p(n!) } (p prime, e_p(n!) odd)
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 2)          /* offset starts at 2; value for n < 2 is undefined */
        return 0;

    /* Generate all primes ≤ n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);
    if (primes == NULL)
        return 0;      /* failure case – unlikely */

    seq_t max_odd = 0;

    for (size_t k = 0; k < prime_cnt; ++k) {
        idx_t p = primes[k];
        idx_t exp = 0;
        idx_t power = p;

        /* Legendre formula: e_p(n!) = sum_{i≥1} floor(n/p^i) */
        while (power <= n) {
            exp += n / power;
            if (power > n / p)   /* avoid overflow; next mul would exceed n */
                break;
            power *= p;
        }

        if ((exp & 1U) && (seq_t)exp > max_odd)
            max_odd = (seq_t)exp;
    }

    free(primes);
    return max_odd;
}
