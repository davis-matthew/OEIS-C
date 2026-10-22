/*
 * A388281  a(n) = A276086(n)*A276086(sigma(n)-n) - A276086(sigma(n)), where
 *          A276086 is the primorial base exp-function, and sigma is the sum
 *          of divisors function.
 * https://oeis.org/A388281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A276086(n)*A276086(sigma(n)-n) - A276086(sigma(n))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 83, u64 n <= 615,
 *            u128 n <= 6889, big any n
 * Helpers:   build_primes_upto, pow_seq, seq_mul, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276086(n) = product of p_i^{a_i}
   where p_i are the primes 2,3,5,7,11,… and a_i are the digits obtained
   by repeatedly dividing n by successive primes starting with 2. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/build_primes_upto.h"
#include "../utils/pow_seq.h"
#include "../utils/seq_mul.h"

static seq_t primorial_exp(idx_t n)
{
    if (n == 0)
        return 1;          /* A276086(0) = 1 */

    static uint64_t *primes = NULL;
    static size_t   prime_cnt = 0;

    if (!primes) {
        /* 200 is enough to cover the first ~46 primes – more than required
           for all n that fit in idx_t (even up to 10^4). */
        build_primes_upto(200, &primes, &prime_cnt);
    }

    seq_t  result = 1;
    idx_t  rem    = n;
    size_t i      = 0;

    while (rem > 0 && i < prime_cnt) {
        uint64_t p    = primes[i];
        idx_t    digit= rem % p;
        result = seq_mul(result, pow_seq((seq_t)p, digit));
        rem    = rem / p;
        ++i;
    }

    /* For safety – if n is huge and we ran out of primes,
       treat the remaining remainder as a digit for the next prime. */
    if (rem > 0 && i < prime_cnt) {
        uint64_t p = primes[i];
        result = seq_mul(result, pow_seq((seq_t)p, rem));
    }

    return result;
}

seq_t a(idx_t n)
{
    seq_t sigma_n     = sigma_of(n);
    idx_t sigma_i     = (idx_t)sigma_n;        /* fits into idx_t for all n used here */
    idx_t diff        = sigma_i - n;          /* sigma(n)-n */

    seq_t val_n   = primorial_exp(n);
    seq_t val_diff= primorial_exp(diff);
    seq_t val_sigma=primorial_exp(sigma_i);

    seq_t prod = seq_mul(val_n, val_diff);
    prod -= val_sigma;

    return prod;
}
