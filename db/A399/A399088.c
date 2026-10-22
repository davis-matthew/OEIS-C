/*
 * A399088  The maximum exponent in the prime factorization of the number
 *          following the n-th squarefree number.
 * https://oeis.org/A399088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum exponent in the prime factorization of the number
 *            following the n-th square‑free number
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Prime sieve – rebuild up to a limit sufficient for the current n.
       Use a new limit each time n is larger than all previously seen values. */
    static uint8_t *is_prime = NULL;
    static size_t prime_limit = 0;
    static uint32_t *prime_list = NULL;
    static size_t prime_count = 0;

    /* Estimate an upper bound for the n‑th squarefree number:
       the density of squarefree integers is 6/pi^2 ≈ 0.6079, so
       n-th squarefree ≈ n / 0.6079 ≈ 1.645 n.  We use 2 n + 20 as a safe
       bound for small n.  The limit we need for primes is the square root
       of the largest candidate we will test (n‑th squarefree+1).          */
    idx_t bound = n * 2 + 20;          /* safe integer bound for n up to about 10 000 */
    size_t needed_prime_limit = bound + 10;

    if (needed_prime_limit > prime_limit)
    {
        /* Re‑sieve up to the new limit */
        free(is_prime);
        free(prime_list);
        prime_limit = needed_prime_limit;

        is_prime = (uint8_t *)malloc((prime_limit + 1) * sizeof(uint8_t));
        if (!is_prime) return 0;   /* allocation failed */

        for (size_t i = 0; i <= prime_limit; ++i) is_prime[i] = 1;
        if (prime_limit >= 0) is_prime[0] = 0;
        if (prime_limit >= 1) is_prime[1] = 0;

        for (size_t i = 2; i * i <= prime_limit; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= prime_limit; j += i)
                    is_prime[j] = 0;

        /* Build prime list */
        prime_count = 0;
        for (size_t i = 2; i <= prime_limit; ++i)
            if (is_prime[i]) ++prime_count;

        prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
        if (!prime_list) return 0;
        size_t pos = 0;
        for (size_t i = 2; i <= prime_limit; ++i)
            if (is_prime[i]) prime_list[pos++] = (uint32_t)i;
    }

    /* Find the n-th squarefree number by scanning consecutively. */
    idx_t count_sqf = 0;
    idx_t candidate = 1;
    while (count_sqf < n)
    {
        uint64_t tmp = candidate;
        bool squarefree = true;
        for (size_t pi = 0; pi < prime_count && (uint64_t)prime_list[pi] * prime_list[pi] <= tmp; ++pi)
        {
            uint32_t p = prime_list[pi];
            uint8_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
                if (exp > 1)
                {
                    squarefree = false;
                    break;
                }
            }
            if (!squarefree) break;
        }
        if (squarefree)
        {
            ++count_sqf;
            if (count_sqf == n) break;
        }
        ++candidate;
    }
    uint64_t squarefree_n = candidate;   /* the n‑th squarefree number */

    /* Factor squarefree_n + 1 and find the highest exponent. */
    uint64_t num = squarefree_n + 1;
    uint8_t max_exp = 0;
    for (size_t pi = 0; pi < prime_count && (uint64_t)prime_list[pi] * prime_list[pi] <= num; ++pi)
    {
        uint32_t p = prime_list[pi];
        uint8_t exp = 0;
        while (num % p == 0)
        {
            num /= p;
            ++exp;
        }
        if (exp > max_exp) max_exp = exp;
    }
    if (num > 1)  /* remaining prime factor */
        if (max_exp < 1) max_exp = 1;

    return (seq_t)max_exp;
}
