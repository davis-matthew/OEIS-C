/*
 * A319880  Difference between 2^n and the product of primes less than or
 *          equal to n.
 * https://oeis.org/A319880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, changed
 * Formula:   a(n) = 2^n - n# (where n# = product of all primes ≤ n)
 * Range:     i8 n <= 8, i16 n <= 15, i32 n <= 28, i64 n <= 52, i128 n <= 100,
 *            big any n
 * Helpers:   build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute the primorial n# */
    seq_t primorial = 1;
    if (n >= 2) {
        uint64_t *primes = NULL;
        size_t prime_count = 0;
        build_primes_upto(n, &primes, &prime_count);
        for (size_t i = 0; i < prime_count; ++i) {
            primorial *= (seq_t)primes[i];
        }
        free(primes);
    }

    /* compute 2^n */
    seq_t two_pow = pow2(n);

    /* return the difference */
    return two_pow - primorial;
}
