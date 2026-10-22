/*
 * A378071  a(n) = denominator((4^(2*n+1)*n!^2)/((2*n+3)*(2*n)!)).
 * https://oeis.org/A378071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator( (4^(2*n+1) * (n!)^2) / ((2*n+3) * (2*n)!) )
 *            * A378071: denominator of the reduced fraction ((4^(2n+1) *
 *            (n!)^2) / * ((2n+3)*(2n)!)). * The sequence starts with a(0)=3.
 *            * * Implementation uses prime factorisation of the numerator and
 *            denominator * and multiplies the primes that...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* cache of primes */
static idx_t *prime_array = NULL;   /* array of primes up to prime_limit */
static size_t  prime_count = 0;     /* number of primes stored */
static idx_t   prime_limit = 0;     /* maximum prime value in array */

/* build a simple sieve up to 'limit' and store all primes ≤ limit in prime_array */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;                       /* already have enough */

    /* sieve */
    size_t size = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(size);
    for (size_t i = 0; i < size; ++i)
        is_prime[i] = true;
    if (size > 0) is_prime[0] = false;
    if (size > 1) is_prime[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t q = p * p; q <= limit; q += p)
                is_prime[q] = false;

    /* count primes */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++count;

    /* allocate and fill prime array */
    idx_t *new_array = (idx_t *)malloc(count * sizeof(idx_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            new_array[pos++] = i;

    free(is_prime);
    free(prime_array);
    prime_array = new_array;
    prime_count = count;
    prime_limit = limit;
}

/* Legendre's formula: exponent of prime p in m! */
static idx_t exp_in_fact(idx_t m, idx_t p)
{
    idx_t exp = 0;
    while (m) {
        m /= p;
        exp += m;
    }
    return exp;
}

/* external function required by the build */
seq_t a(idx_t n)
{
    idx_t limit = 2 * n + 3;
    ensure_primes(limit);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_array[i];
        if (p > limit)
            break;                        /* safety guard */

        /* exponent of p in the numerator:
           4^(2n+1) contributes 2^(4n+2), so if p==2 add that.
           (n!)^2 contributes 2*exp_in_fact(n,p). */
        idx_t exp_num = 2 * exp_in_fact(n, p);
        if (p == 2)
            exp_num += 4 * n + 2;

        /* exponent of p in the denominator:
           (2n)! contributes exp_in_fact(2n,p).
           (2n+3) contributes its multiplicity of p. */
        idx_t exp_den = exp_in_fact(2 * n, p);

        idx_t temp    = 2 * n + 3;
        idx_t factor_exp = 0;
        while (temp % p == 0) {
            ++factor_exp;
            temp /= p;
        }
        exp_den += factor_exp;

        /* if denominator exponent exceeds numerator exponent,
           multiply the prime into the result that many times. */
        if (exp_den > exp_num) {
            idx_t diff = exp_den - exp_num;
            for (idx_t j = 0; j < diff; ++j)
                result *= (seq_t)p;
        }
    }

    return result;
}
