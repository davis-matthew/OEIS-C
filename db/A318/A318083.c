/*
 * A318083  Numbers m such that sigma(sigma(m))/sigma(m) <= sigma(m)/m.
 * https://oeis.org/A318083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 2424, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): numbers m such that σ(σ(m))/σ(m) <= σ(m)/m */
/* The function tests successive integers and memoises those that satisfy
   the inequality.  It uses trial‑division with a prime sieve up to
   a fixed limit.  The implementation is valid C11 and C++17 alike. */

#include <stdint.h>
#include <stdlib.h>

/* ---------- prime sieve -------------------------------------------------- */
static void build_primes(uint32_t limit, uint32_t **primes_out, size_t *cnt_out)
{
    /* Allocate a boolean array (one byte per entry).  Cast required for C++. */
    unsigned char *is_prime = (unsigned char *)calloc(limit + 1, 1);
    if (!is_prime) {
        *primes_out = (uint32_t *)NULL;
        *cnt_out    = 0;
        return;
    }
    for (uint32_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }
    /* Count primes to allocate exact size. */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint32_t *primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) {
        free(is_prime);
        *primes_out = (uint32_t *)NULL;
        *cnt_out    = 0;
        return;
    }
    size_t k = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[k++] = i;
    free(is_prime);
    *primes_out = primes;
    *cnt_out    = cnt;
}

/* ---------- sigma function for uint64_t --------------------------------- */
static uint64_t sigma_uint64(uint64_t n, const uint32_t *primes, size_t prime_cnt)
{
    uint64_t result = 1;
    uint64_t temp   = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            uint64_t term  = 1;
            uint64_t power = 1;
            while (temp % p == 0) {
                temp   /= p;
                power  *= p;
                term   += power;
            }
            result *= term;
        }
    }
    if (temp > 1) {
        /* Remaining prime factor. */
        result *= (temp + 1);
    }
    return result;
}

/* ---------- sequence generator ------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *terms   = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* Static state for search. */
    static uint64_t              next_m     = 1;            /* smallest m not yet tested */
    static uint32_t             *primes_list = NULL;       /* list of primes          */
    static size_t                prime_cnt   = 0;            /* prime count             */
    static int                   initialized = 0;          /* flag if primes ready    */

    /* Initialise primes once (sieve up to a safe limit). */
    if (!initialized) {
        /* 2 000 000 is sufficient for n up to about 10 000. */
        build_primes(2000000, &primes_list, &prime_cnt);
        /* Explicit cast for C++: */
        primes_list = (uint32_t *)primes_list;
        initialized = 1;
    }

    /* Grow the terms cache until it contains the requested index. */
    while (terms_len < (size_t)n) {
        uint64_t m = next_m;
        /* Compute σ(m). */
        uint64_t sigma_m = sigma_uint64(m, primes_list, prime_cnt);
        /* Compute σ(σ(m)).  Here σ_m fits in 64 bits for all m tested. */
        uint64_t sigma_sigma = sigma_uint64(sigma_m, primes_list, prime_cnt);

        /* Inequality: σ(σ(m)) * m <= σ(m)^2  */
        unsigned __int128 left  = (unsigned __int128)sigma_sigma * m;
        unsigned __int128 right = (unsigned __int128)sigma_m * sigma_m;

        if (left <= right) {
            /* Append m to cache as seq_t. */
            if (terms_len + 1 > terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                /* Cast required for C++. */
                seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_arr) exit(1);  /* allocation failure */
                terms  = new_arr;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)m;
        }
        ++next_m;
    }

    return terms[n - 1];
}
