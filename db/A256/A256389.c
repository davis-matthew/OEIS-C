/*
 * A256389  Numbers n such that one or more primes can be the arithmetic mean
 *          of 2 semiprimes whose difference is 2*n.
 * https://oeis.org/A256389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A256389:
   a(n) = the n-th positive integer k for which there exists a prime p
   such that both p-k and p+k are semiprime (product of two primes,
   with possible repetition).  The function returns the terms in
   ascending order.  The implementation uses a sieve to pre‑compute
   all primes and all semiprimes up to a fixed limit, then
   searches for the qualifying k values and caches the results.
   The cache is kept between calls, so the function works
   correctly for any call order. */

/* ------------------------------------------------------------ Imports */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ------------------------------------------------------------ Global data */
#define LIMIT 10000000U   /* Sieve bound – enough for n up to ~10 000 */

static bool *is_prime   = NULL;     /* is_prime[i] – i is prime     */
static bool *is_semiprime = NULL;   /* is_semiprime[i] – i is semiprime */
static uint32_t *primes = NULL;     /* array of primes */
static size_t primes_cnt = 0;

/* Cached sequence */
static seq_t *seq_cache = NULL;     /* values of a(n) */
static idx_t  seq_len   = 0;        /* number of cached terms   */
static size_t seq_cap   = 0;        /* allocated capacity       */
static idx_t  next_candidate = 1;   /* next integer to test     */

/* ------------------------------------------------------------ Utility */
static void ensure_seq_capacity(idx_t need)
{
    if ((size_t)need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------ Sieve */
static void build_sieve(void)
{
    /* Allocate arrays */
    is_prime     = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    is_semiprime = (bool *)malloc((LIMIT + 1) * sizeof(bool));

    /* Initialise prime array */
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; (idx_t)i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    /* Collect primes */
    primes_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++primes_cnt;

    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    size_t pidx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[pidx++] = (uint32_t)i;

    /* Initialise semiprime array */
    for (idx_t i = 0; i <= LIMIT; ++i) is_semiprime[i] = false;

    /* Mark all products of two primes (unordered pairs) */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > LIMIT) break;
        for (size_t j = i; j < primes_cnt; ++j) {
            uint32_t q = primes[j];
            uint64_t prod = (uint64_t)p * q;
            if (prod > LIMIT) break;
            is_semiprime[(idx_t)prod] = true;
        }
    }
}

/* ------------------------------------------------------------ Search */
static bool has_valid_prime(idx_t n)
{
    /* For each prime p check if p-n and p+n are semiprime */
    for (size_t ii = 0; ii < primes_cnt; ++ii) {
        uint32_t p = primes[ii];
        idx_t p_val = (idx_t)p;
        if (p_val + n > LIMIT) break;       /* cannot test further */
        if (p_val < n + 4) continue;        /* p-n would be < 4      */
        if (is_semiprime[p_val - n] && is_semiprime[p_val + n])
            return true;
    }
    return false;
}

/* ------------------------------------------------------------ Main function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        build_sieve();
        seq_cache = (seq_t *)malloc(128 * sizeof(seq_t));
        seq_cap   = 128;
        seq_len   = 0;
        initialized = true;
    }

    /* Extend cache until we have at least n elements */
    while (seq_len < n) {
        while (!has_valid_prime(next_candidate)) ++next_candidate;
        ensure_seq_capacity(seq_len + 1);
        seq_cache[seq_len] = (seq_t)next_candidate;
        ++seq_len;
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
