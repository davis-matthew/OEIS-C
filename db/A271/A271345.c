/*
 * A271345  Integers n such that (n-1)! is divisible by n^3.
 * https://oeis.org/A271345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Integers n such that (n-1)! is divisible by n^3
 * Range:     u8 n <= 145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const idx_t SIEVE_LIMIT = 20000000u;

static unsigned char *sieve = NULL;
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

static void init_sieve(void)
{
    if (sieve != NULL)
        return;

    sieve = (unsigned char*)malloc((size_t)SIEVE_LIMIT + 1);
    if (!sieve)
        return;

    for (size_t i = 0; i <= (size_t)SIEVE_LIMIT; ++i)
        sieve[i] = 0;

    for (size_t i = 2; (size_t)i * i <= (size_t)SIEVE_LIMIT; ++i)
    {
        if (!sieve[i])
        {
            for (size_t j = i * i; j <= (size_t)SIEVE_LIMIT; j += i)
                sieve[j] = 1;
        }
    }

    size_t approx = (size_t)SIEVE_LIMIT / 10;
    primes = (uint32_t*)malloc(approx * sizeof(uint32_t));
    if (!primes)
        return;

    for (size_t i = 2; i <= (size_t)SIEVE_LIMIT; ++i)
    {
        if (!sieve[i])
            primes[primes_cnt++] = (uint32_t)i;
    }
}

static bool candidate_ok(idx_t cand)
{
    if (cand <= 1)
        return true;

    idx_t temp = cand;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > temp)
            break;
        if (temp % p == 0)
        {
            size_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            idx_t exp_fact = 0;
            idx_t t = cand - 1;
            while (t)
            {
                t /= p;
                exp_fact += t;
            }

            if (exp_fact < (idx_t)3 * e)
                return false;
        }
    }

    if (temp > 1)
    {
        idx_t p = temp;
        idx_t exp_fact = 0;
        idx_t t = cand - 1;
        while (t)
        {
            t /= p;
            exp_fact += t;
        }
        if (exp_fact < 3)
            return false;
    }

    return true;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static idx_t next_candidate = 2;

    init_sieve();

    if (terms == NULL)
    {
        terms = (seq_t*)malloc(sizeof(seq_t));
        if (!terms)
            return 0;
        terms[0] = (seq_t)1; /* a(1) */
        terms_len = 1;
    }

    while (terms_len < (size_t)n)
    {
        while (!candidate_ok(next_candidate))
            ++next_candidate;

        size_t new_len = terms_len + 1;
        seq_t *tmp = (seq_t*)realloc(terms, new_len * sizeof(seq_t));
        if (!tmp)
            return 0;
        terms = tmp;

        terms[terms_len] = (seq_t)next_candidate;
        ++terms_len;
        ++next_candidate;
    }

    return terms[n - 1];
}
