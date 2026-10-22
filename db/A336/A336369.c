/*
 * A336369  Primes p(n) such that gcd(n, prime(n)+prime(n+1)) > 1.
 * https://oeis.org/A336369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of primes p(n) such that gcd(n, p(n)+p(n+1)) > 1. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd_uint64.h"

/* -------------------------------------------------------------------- */
/*   Simple sieve: keeps a static list of primes up to a dynamic limit   */
/* -------------------------------------------------------------------- */

static uint64_t *sieve_primes = NULL;   /* array of primes */
static size_t sieve_count   = 0;        /* number of primes in array */
static uint64_t sieve_limit = 0;        /* current maximum sieved value */

/* Extends the sieve so that primes up to 'limit' are computed and stored.
   It replaces the previous prime list. */
static void extend_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;          /* already sufficient */

    /* allocate temporary sieve array (char works as boolean) */
    char *sieve = (char*)malloc(limit + 1);
    if (!sieve) exit(1);                       /* out of memory */

    for (uint64_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (sieve[p]) {
            for (uint64_t x = p * p; x <= limit; x += p) sieve[x] = 0;
        }
    }

    /* count primes */
    size_t new_count = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (sieve[i]) ++new_count;

    /* allocate new prime array */
    uint64_t *new_primes = (uint64_t*)malloc(new_count * sizeof(uint64_t));
    if (!new_primes) exit(1);
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (sieve[i]) new_primes[idx++] = i;

    /* free previous resources */
    if (sieve_primes) free(sieve_primes);
    if (sieve) free(sieve);

    /* store new data */
    sieve_primes = new_primes;
    sieve_count  = new_count;
    sieve_limit  = limit;
}

/* -------------------------------------------------------------------- */
/*                               GCD helpers                           */
/* -------------------------------------------------------------------- */

/* -------------------------------------------------------------------- */
/*                     Main sequence function (a)                      */
/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined below offset */

    /* static state shared across calls */
    static seq_t *match = NULL;    /* array of matched primes */
    static size_t match_cnt = 0;   /* number of matched primes stored */
    static size_t match_cap = 0;
    static idx_t next_idx = 0;     /* zero‑based index into sieve_primes */
    static uint64_t local_limit = 0; /* current sieve limit */

    /* initialise match array on first call */
    if (match == NULL) {
        match_cap = 16;
        match = (seq_t*)malloc(match_cap * sizeof(seq_t));
        if (!match) exit(1);
    }

    /* repeatedly collect primes until we have at least n matches */
    while (match_cnt < n) {
        /* make sure we have primes up to next_idx+1 */
        while ((size_t)next_idx + 1 >= sieve_count) {
            if (local_limit == 0) local_limit = 100;
            else local_limit <<= 1;         /* double the limit */
            extend_sieve(local_limit);
        }

        uint64_t p_n  = sieve_primes[(size_t)next_idx];
        uint64_t p_n1 = sieve_primes[(size_t)next_idx + 1];

        uint64_t sum = p_n + p_n1;
        uint64_t g   = gcd_uint64((uint64_t)(next_idx + 1), sum);

        if (g > 1) {
            /* store p_n into the result array */
            if (match_cnt == match_cap) {
                match_cap = match_cap? match_cap * 2 : 16;
                match = (seq_t*)realloc(match, match_cap * sizeof(seq_t));
                if (!match) exit(1);
            }
            match[match_cnt] = (seq_t)p_n;
            ++match_cnt;
        }

        ++next_idx;
    }

    return match[n - 1];
}
