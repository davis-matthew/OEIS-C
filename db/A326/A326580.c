/*
 * A326580  a(n) = (2*n+1)*denominator((2*n+1)*Bernoulli(2*n)).
 * https://oeis.org/A326580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)*denominator((2*n+1)*Bernoulli(2*n))
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 59, u64 n <= 269,
 *            u128 n <= 1889, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;   /* primes up to max_limit */
static size_t   prime_cnt = 0;    /* number of primes stored */
static idx_t    max_limit = 0;    /* largest limit we have cached */

seq_t a(idx_t n)
{
    /* work in seq_t to avoid overflow of intermediate results */
    idx_t mp1_i = (idx_t)(2*n + 1);          /* integer value of 2*n+1 */
    seq_t mp1   = (seq_t)mp1_i;                /* same value as seq_t */
    seq_t m     = (seq_t)(2*n);               /* 2*n as seq_t */

    /* ensure we have primes up to mp1_i */
    if (mp1_i > max_limit) {
        if (primes) free(primes);
        build_primes_upto(mp1_i, &primes, &prime_cnt);
        max_limit = mp1_i;
    }

    /* a(n) = (2*n+1) * product of primes p ≤ 2*n+1
       with (p-1) | 2*n and p ∤ (2*n+1)                          */
    seq_t result = mp1;                           /* start with (2*n+1) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if ((seq_t)p > mp1) break;                /* primes are sorted */
        if ((m % (seq_t)(p-1)) == 0) {            /* (p-1) divides 2*n? */
            if (mp1_i % p != 0) {                /* p does NOT divide 2*n+1? */
                result *= (seq_t)p;               /* multiply by p */
            }
        }
    }
    return result;
}
