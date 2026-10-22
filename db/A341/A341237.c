/*
 * A341237  a(k) is the number of pairs of consecutive primes (p,q) such that
 *          (2*k-q,2*k-p) is also a pair of consecutive primes.
 * https://oeis.org/A341237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5459, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(k) = #{ (p,q)=consecutive primes | (2*k-q, 2*k-p) also consecutive primes }  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static size_t prime_limit = 0;         /* current sieve limit (inclusive)            */
static uint8_t *isprime = NULL;        /* 0/1 array for primality test                */
static uint64_t *primes = NULL;        /* array of primes up to prime_limit          */
static int *prime_index = NULL;        /* mapping from number->index in primes array */
static size_t primes_cnt = 0;          /* number of primes found                       */

/* ------------------------------------------------------------------ */
/*  Expands the prime table to cover all integers up to 'lim'          */
/* ------------------------------------------------------------------ */
static void ensure_primes(size_t lim)
{
    if (lim <= prime_limit)
        return;                     /* nothing to do */

    /* free old data if any */
    free(isprime);
    free(primes);
    free(prime_index);

    /* Allocate arrays */
    isprime = (uint8_t *)malloc((lim + 1) * sizeof(uint8_t));
    if (!isprime)
        return; /* caller will handle failure; should never happen with small limits */

    prime_index = (int *)malloc((lim + 1) * sizeof(int));
    if (!prime_index) {
        free(isprime);
        return;
    }

    size_t alloc_primes = lim / 2 + 10;                 /* rough upper bound      */
    primes = (uint64_t *)malloc(alloc_primes * sizeof(uint64_t));
    if (!primes) {
        free(isprime);
        free(prime_index);
        return;
    }

    /* Sieve of Eratosthenes */
    for (size_t i = 0; i <= lim; ++i)
        isprime[i] = 1;
    isprime[0] = isprime[1] = 0;
    for (size_t i = 2; i * i <= lim; ++i)
        if (isprime[i])
            for (size_t j = i * i; j <= lim; j += i)
                isprime[j] = 0;

    /* Build prime list and index map */
    primes_cnt = 0;
    for (int i = 0; i <= (int)lim; ++i)
        prime_index[i] = -1;                /* not a prime            */

    for (size_t i = 2; i <= lim; ++i) {
        if (isprime[i]) {
            if (primes_cnt >= alloc_primes) {   /* safety: expand if needed */
                alloc_primes *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, alloc_primes * sizeof(uint64_t));
                if (!tmp) break;
                primes = tmp;
            }
            primes[primes_cnt] = i;
            prime_index[i] = (int)primes_cnt;
            ++primes_cnt;
        }
    }

    prime_limit = lim;     /* update after successful construction   */
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1; return 0 for safety */
        return 0;

    idx_t k = n;                           /* local copy, unsigned long long   */
    size_t limit = (size_t)k * 2ULL;        /* sieve all numbers up to 2*k      */

    ensure_primes(limit);
    if (!isprime)           /* memory allocation failure */
        return 0;

    seq_t count = 0;

    /* iterate over consecutive prime pairs */
    for (size_t i = 0; i + 1 < primes_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t q = primes[i + 1];

        uint64_t p_rev = (uint64_t)(2 * k) - q;
        uint64_t q_rev = (uint64_t)(2 * k) - p;

        if (p_rev < 2 || q_rev > limit || p_rev >= q_rev)
            continue;

        /* check primality */
        if (!isprime[p_rev] || !isprime[q_rev])
            continue;

        /* check consecutiveness */
        int idx_p = prime_index[(int)p_rev];
        int idx_q = prime_index[(int)q_rev];
        if (idx_p >= 0 && idx_q == idx_p + 1)
            ++count;
    }

    return count;
}
