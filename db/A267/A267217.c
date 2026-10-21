/*
 * A267217  10-gonal (or decagonal) numbers with prime indices.
 * https://oeis.org/A267217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) * (4*prime(n) - 3) (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static helpers for prime generation */
static bool *sieve = NULL;          /* sieve table */
static uint32_t *primes = NULL;     /* array of primes */
static size_t primes_used = 0;      /* number of primes currently stored */
static idx_t sieve_limit = 0;       /* current upper bound of the sieve */

/* ensure we have at least n primes stored; re‑sieve if necessary */
static void ensure_primes(idx_t n)
{
    /* no work if we already have enough primes */
    if ((size_t)n <= primes_used) return;

    /* initial sieve limit */
    if (sieve_limit == 0) sieve_limit = 200000;

    while (1) {
        /* expand sieve buffer to current limit */
        sieve = (bool *)realloc(sieve, sieve_limit + 1);
        if (!sieve) exit(EXIT_FAILURE);

        /* mark all as prime initially */
        for (size_t i = 0; i <= (size_t)sieve_limit; ++i) sieve[i] = false;
        if (sieve_limit >= 0) sieve[0] = true;          /* 0 not prime */
        if (sieve_limit >= 1) sieve[1] = true;          /* 1 not prime */

        /* classic sieve of Eratosthenes */
        for (size_t i = 2; i * i <= (size_t)sieve_limit; ++i) {
            if (!sieve[i]) {
                for (size_t j = i * i; j <= (size_t)sieve_limit; j += i)
                    sieve[j] = true;
            }
        }

        /* collect primes up to the requested count n */
        primes = (uint32_t *)realloc(primes, (size_t)n * sizeof(uint32_t));
        if (!primes) exit(EXIT_FAILURE);

        size_t count = 0;
        for (size_t i = 2; i <= (size_t)sieve_limit && count < (size_t)n; ++i) {
            if (!sieve[i]) {
                primes[count++] = (uint32_t)i;
            }
        }

        if (count >= (size_t)n) {
            primes_used = count;
            break;
        }
        /* not enough primes – double the limit and retry */
        sieve_limit *= 2;
    }
}

/* sequence A267217: 10‑gonal numbers with prime indices */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined below */

    ensure_primes(n);
    seq_t p  = (seq_t)primes[n - 1];                /* prime(n) */
    seq_t res = p * ((seq_t)4 * p - (seq_t)3);      /* a(n) = p*(4p-3) */
    return res;
}
