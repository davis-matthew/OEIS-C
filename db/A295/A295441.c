/*
 * A295441  a(n) = (9*n)!*n!/((5*n)!*(3*n)!*(2*n)!).
 * https://oeis.org/A295441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9*n)!*n! / ((5*n)!*(3*n)!*(2*n)!)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   _prime_exp, build_primes_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_uint.h"
#include "../utils/_prime_exp.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t nine  = 9 * n;
    idx_t five  = 5 * n;
    idx_t three = 3 * n;
    idx_t two   = 2 * n;

    /* Sieve of Eratosthenes up to 9*n to obtain all primes needed */
    uint64_t *primes = NULL;
    size_t    prime_cnt = 0;
    build_primes_upto(nine, &primes, &prime_cnt);

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        idx_t e = _prime_exp(nine, p) + _prime_exp(n, p)
                  - _prime_exp(five, p) - _prime_exp(three, p)
                  - _prime_exp(two, p);
        if (e == 0)
            continue;
        seq_t p_pow = pow_uint((uint64_t)p, e);
        result *= p_pow;
    }

    free(primes);
    return result;
}
