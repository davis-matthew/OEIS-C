/*
 * A352169  Numbers k with a Goldbach partition (p,q) such that k/2 divides at
 *          least one of p*q +- 1.
 * https://oeis.org/A352169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352169
 * Even integers k ≥ 8 such that a Goldbach partition (p,q) of k satisfies
 * k/2 | (p*q – 1) or k/2 | (p*q + 1).
 * The function incrementally searches even numbers and returns the n‑th
 * element (1‑indexed).  No closed formula is known, so a brute‑force
 * sieve‑based test is used.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------- */
/*  Prime sieve data                                           */
/* ------------------------------------------------------------- */
static uint8_t *is_prime = NULL;       /* 0/1 array up to sieve_limit */
static idx_t   sieve_limit = 0;        /* highest value in the sieve */

static uint64_t *primes = NULL;        /* list of primes up to sieve_limit */
static size_t   primes_cnt = 0;        /* number of stored primes */

/* Build a simple sieve of Eratosthenes up to 'limit' (inclusive) */
static void build_sieve(idx_t limit)
{
    if (limit < 2)
        return;

    /* Allocate a fresh sieve array */
    uint8_t *sieve = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!sieve)
        abort();                       /* out of memory */
    sieve[0] = 0;
    sieve[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        sieve[i] = 1;

    /* Mark composites */
    for (idx_t i = 2; i <= limit / i; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;
        }
    }

    /* Count primes to allocate the primes array */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            ++cnt;

    /* Reallocate the primes array to the required size */
    uint64_t *pr = (uint64_t *)realloc(primes, cnt * sizeof(uint64_t));
    if (!pr)
        abort();                       /* out of memory */
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            pr[pos++] = i;

    /* Free old sieve and install the new one */
    free(is_prime);
    is_prime = sieve;
    sieve_limit = limit;
    primes = pr;
    primes_cnt = cnt;
}

/* Ensure that the sieve covers all numbers up to 'limit'.  If it is
 * already large enough nothing is done. */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;
    build_sieve(limit);
}

/* ------------------------------------------------------------- */
/*  Sequence cache                                              */
/* ------------------------------------------------------------- */
static seq_t *cache = NULL;            /* dynamic array of terms */
static size_t cache_len = 0;            /* number of terms stored */
static size_t cache_cap = 0;            /* allocated capacity */

/* Append a term to the cache, growing the array if necessary */
static void append_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr)
            abort();
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------- */
/*  Goldbach property test                                      */
/* ------------------------------------------------------------- */
static bool satisfies_property(idx_t k)
{
    if (k < 8 || (k & 1))
        return false;

    ensure_sieve(k);
    idx_t half = k >> 1;               /* k/2 */

    for (size_t i = 0; i < primes_cnt && primes[i] <= half; ++i) {
        uint64_t p = primes[i];
        uint64_t q = k - p;
        if (!is_prime[q])
            continue;

        unsigned __int128 prod = (unsigned __int128)p * q;
        unsigned __int128 h    = (unsigned __int128)half;

        if (((prod + 1) % h == 0) || ((prod - 1) % h == 0))
            return true;
    }
    return false;
}

/* ------------------------------------------------------------- */
/*  Main function                                                */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined; safe return */

    static idx_t next_candidate = 8;   /* next even number to test */

    while (cache_len < n) {
        if (satisfies_property(next_candidate))
            append_term((seq_t)next_candidate);
        next_candidate += 2;
    }
    return cache[n - 1];
}
