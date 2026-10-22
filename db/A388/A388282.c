/*
 * A388282  a(n) = A276086(n) * A276086(sigma(n)-n), where A276086 is the
 *          primorial base exp-function, and sigma is the sum of divisors
 *          function.
 * https://oeis.org/A388282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276086(n) * A276086(sigma(n)-n)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 83, u64 n <= 615,
 *            u128 n <= 6889, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sigma_of.h"

/*--- prime cache ------------------------------------------------------*/
static uint64_t *prime_cache = NULL; /* array of primes */
static size_t   prime_cnt  = 0;      /* number of primes stored */
static size_t   prime_cap  = 0;      /* allocated capacity          */
static uint64_t next_candidate = 2;  /* next integer to test */

/* Extend the prime cache until it contains at least 'need' primes. */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need)
        return;

    while (prime_cnt < need)
    {
        uint64_t p = next_candidate;
        bool ok = true;

        /* Simple trial division using existing primes */
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t q = prime_cache[i];
            if (q * q > p)      /* no more primes to try */
                break;
            if (p % q == 0) { /* composite */
                ok = false;
                break;
            }
        }

        if (ok)
        {
            if (prime_cnt == prime_cap)
            {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
                if (!tmp)
                    abort();           /* allocation failure */
                prime_cache = tmp;
                prime_cap   = new_cap;
            }
            prime_cache[prime_cnt++] = p;
        }

        /* Advance candidate: after 2, skip even numbers */
        if (p == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

/*--- A276086 (primorial base exponent function) ---------------------*/
static seq_t primorial_exp(idx_t n)
{
    seq_t result = (seq_t)1;
    idx_t x = n;
    size_t idx = 0;

    while (x > 0)
    {
        ensure_primes(idx + 1);
        uint64_t p = prime_cache[idx];
        idx_t digit = x % p;                 /* 0 <= digit < p           */

        /* multiply prime^digit */
        seq_t term = (seq_t)1;
        for (idx_t k = 0; k < digit; ++k)
            term *= (seq_t)p;

        result *= term;
        x /= p;
        ++idx;
    }
    return result;
}

/*--- a(n) -------------------------------------------------------------*/
seq_t a(idx_t n)
{
    seq_t v1 = primorial_exp(n);
    seq_t sigma = sigma_of(n);                 /* sigma(n)  */
    seq_t delta = sigma - (seq_t)n;
    idx_t rest = (idx_t)delta;                 /* sigma(n)-n */
    seq_t v2 = primorial_exp(rest);
    return v1 * v2;
}
