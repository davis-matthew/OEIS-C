/*
 * A248771  Greatest k such that k^6 divides n!
 * https://oeis.org/A248771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏ p^{⌊v_p(n!)/6⌋} (product over all primes p ≤ n)
 * Range:     u8 n <= 26, u16 n <= 45, u32 n <= 74, u64 n <= 113,
 *            u128 n <= 197, big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
/* The helper functions used below are supplied by the build. */
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1. */
    if (n < 1)
        return (seq_t)1;

    /* Build the list of primes up to n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* Compute the exponent of p in n! (Legendre's formula). */
        idx_t  pow  = p;
        uint64_t e   = 0;                /* v_p(n!) */

        while (pow <= n) {
            e += n / pow;
            if (pow > n / p)            /* avoid overflow of pow * p   */
                break;
            pow *= p;
        }

        uint64_t exp = e / 6;           /* exponent of p in the answer */
        if (exp > 0) {
            seq_t pk = big_pow((seq_t)p, (seq_t)exp);
            result *= pk;
        }
    }

    free(primes);  /* clean up the array allocated by build_primes_upto */
    return result;
}
