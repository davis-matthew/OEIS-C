/*
 * A303557  a(0) = 1; a(n) = 2^(n-1)*prime(n)#, where prime(n)# is the product
 *          of first n primes.
 * https://oeis.org/A303557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1)*prime(n)#
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   seq_mul, seq_pow, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* obtain the first n primes */
    uint32_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_first_n_primes(n, &primes, &prime_cnt);

    /* product of the first n primes */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        prod = seq_mul(prod, (seq_t)primes[i]);
    }

    /* multiply by 2^(n-1) */
    seq_t p2 = seq_pow((seq_t)2, n - 1);
    prod = seq_mul(prod, p2);

    /* clean up */
    free(primes);

    return prod;
}
