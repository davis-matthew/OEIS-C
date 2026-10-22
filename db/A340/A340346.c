/*
 * A340346  The largest divisor of n that is a term of A055932 (numbers
 *          divisible by all primes smaller than their largest prime factor).
 * https://oeis.org/A340346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest divisor of n that is a term of A055932 (numbers
 *            divisible by all primes smaller than their largest prime factor)
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 2) /* a(1) = 1 */
        return (seq_t)1;

    /* Step 1: factorize n */
    uint64_t primes_arr[20];
    uint64_t exps_arr[20];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes_arr, exps_arr, &prime_cnt);

    /* Step 2: build list of all primes up to the greatest prime factor of n */
    uint64_t gpf = primes_arr[prime_cnt - 1];
    uint64_t *prime_upto = NULL;
    size_t upto_cnt = 0;
    build_primes_upto(gpf, &prime_upto, &upto_cnt);

    /* Step 3: iterate over the factor list; use only prefixes that contain
       all small primes with no gaps */
    seq_t best = (seq_t)1;
    seq_t cur   = (seq_t)1;
    int prefix_ok = 1;               /* 1 = all previous primes matched */

    for (size_t i = 0; i < prime_cnt; ++i) {
        /* Raise prime to its exponent */
        for (uint64_t e = 0; e < exps_arr[i]; ++e)
            cur *= (seq_t)primes_arr[i];

        /* Check prefix condition */
        if (prefix_ok && primes_arr[i] == prime_upto[i]) {
            /* All primes <= current prime are present */
            if (cur > best)
                best = cur;
        } else {
            prefix_ok = 0;            /* missing a required small prime */
            break;            /* further primes cannot satisfy condition */
        }
    }

    /* Clean up */
    free(prime_upto);

    return best;
}
