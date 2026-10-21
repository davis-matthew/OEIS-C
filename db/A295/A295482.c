/*
 * A295482  a(n) =
 *          (24*n)!*(9*n)!*(6*n)!*(4*n)!/((18*n)!*(12*n)!*(8*n)!*(3*n)!*(2*n)!).
 * https://oeis.org/A295482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            binomial(24*n,6*n)*binomial(6*n,3*n)*binomial(4*n,2*n)/(binomial(12*n,3*n)*binomial(8*n,2*n))
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   build_primes_upto, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_int.h"

/* Legendre's formula: exponent of prime p in m! */
static idx_t exp_factorial(idx_t m, uint64_t p)
{
    idx_t e = 0;
    while (m)
    {
        m /= p;
        e += m;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Prime table cache */
    static uint64_t *prime_table = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    idx_t limit = 24ull * n;
    if (prime_table == NULL || limit > prime_limit)
    {
        /* Rebuild prime table if needed */
        if (prime_table)
        {
            free(prime_table);
            prime_table = NULL;
            prime_cnt = 0;
        }
        build_primes_upto(limit, &prime_table, &prime_cnt);
        prime_limit = limit;
    }

    seq_t res = 1;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_table[i];
        idx_t e = 0;

        e += exp_factorial(24ull * n, p);
        e += exp_factorial(9ull * n, p);
        e += exp_factorial(6ull * n, p);
        e += exp_factorial(4ull * n, p);

        e -= exp_factorial(18ull * n, p);
        e -= exp_factorial(12ull * n, p);
        e -= exp_factorial(8ull * n, p);
        e -= exp_factorial(3ull * n, p);
        e -= exp_factorial(2ull * n, p);

        if (e > 0)
        {
            res *= pow_int((seq_t)p, e);
        }
        /* Exponents should never be negative for this sequence */
    }

    return res;
}
