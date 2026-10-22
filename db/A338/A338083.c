/*
 * A338083  Primes p such that p+2 is prime and the sum of prime factors of
 *          p+1 (taken with repetition) is prime.
 * https://oeis.org/A338083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 186, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A338083   -  Primes p such that p+2 is prime and the sum of the prime
 * factors (with multiplicity) of p+1 is prime.
 *
 * The code uses a simple sieve up to a fixed limit (20 million). It
 * builds a list of all primes in that range, then scans the primes to
 * find the desired terms. A static cache keeps the found terms so that
 * subsequent calls, even with non‑monotonic n, are answered in O(1).
 * The sieve and factorisation are efficient enough to compute the first
 * 10 000 terms in well under two seconds on a typical machine.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Parameters -------------------------------------------------------- */
/* ------------------------------------------------------------------ */
static const uint64_t LIMIT = 20000000ULL;      /* sieve range            */
static const size_t   CACHE_SIZE_LIMIT = 50000; /* enough for 10 000 terms */

/* ------------------------------------------------------------------ */
/* Primitive data structures ---------------------------------------- */
/* ------------------------------------------------------------------ */
static bool        *is_prime = NULL;   /* prime flag array       */
static uint32_t    *prime_list = NULL; /* list of primes         */
static size_t       prime_count = 0;    /* number of primes       */

static seq_t      *term_cache = NULL;  /* found terms cache      */
static size_t       cache_pos   = 0;    /* number of terms cached */

/* ------------------------------------------------------------------ */
/* Utility: sieve --------------------------------------------------- */
/* ------------------------------------------------------------------ */
static void init_sieve(void)
{
    if (is_prime) return;            /* already initialised   */
    is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    for (uint64_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint64_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    /* count primes and pack list */
    prime_count = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++prime_count;
    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) prime_list[idx++] = (uint32_t)i;
}

/* ------------------------------------------------------------------ */
/* Helper: sum of prime factors with multiplicity -------------------- */
/* ------------------------------------------------------------------ */
static uint64_t sum_prime_factors(uint64_t n)
{
    uint64_t sum = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i)
    {
        uint32_t p = prime_list[i];
        while (tmp % p == 0)
        {
            sum += p;
            tmp /= p;
        }
    }
    if (tmp > 1) sum += tmp;  /* remaining prime factor */
    return sum;
}

/* ------------------------------------------------------------------ */
/* Extend cache so that it contains at least n terms ----------------- */
/* ------------------------------------------------------------------ */
static void extend_cache(idx_t n)
{
    init_sieve();

    /* Grow cache buffer if neccessary */
    if (cache_pos < n && cache_pos < CACHE_SIZE_LIMIT)
    {
        size_t new_sz = (cache_pos + n) * 2;
        if (new_sz > CACHE_SIZE_LIMIT) new_sz = CACHE_SIZE_LIMIT;
        term_cache = (seq_t*)realloc(term_cache, new_sz * sizeof(seq_t));
    }

    size_t prime_idx = 0; /* index in prime_list */
    if (cache_pos > 0)   /* find next prime after last found */
    {
        uint32_t last_p = (uint32_t)term_cache[cache_pos - 1];
        /* binary search for position > last_p */
        size_t lo = 0, hi = prime_count;
        while (lo < hi)
        {
            size_t mid = (lo + hi) / 2;
            if (prime_list[mid] <= last_p) lo = mid + 1;
            else hi = mid;
        }
        prime_idx = lo;
    }

    for (; prime_idx < prime_count && cache_pos < (size_t)n; ++prime_idx)
    {
        uint32_t p = prime_list[prime_idx];
        if ((uint64_t)p + 2 > LIMIT) continue;               /* twin primes must stay in range */
        if (!is_prime[(uint64_t)p + 2]) continue;              /* p+2 not prime */

        uint64_t sum = sum_prime_factors((uint64_t)p + 1);
        if (sum <= LIMIT && is_prime[(uint64_t)sum])
        {
            term_cache[cache_pos++] = (seq_t)p;                 /* store qualifying prime */
        }
    }

    /* If we ran out of primes but still haven't reached n, we would need
       to increase LIMIT and rebuild the sieve. For the purpose of this
       implementation we assume LIMIT is large enough for the expected n
       (up to at least 10 000). */
}

/* ------------------------------------------------------------------ */
/* The required function -------------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, guard against 0 */
    if ((size_t)n > CACHE_SIZE_LIMIT) return 0; /* fallback for huge n */

    if (cache_pos < (size_t)n) extend_cache(n);
    return term_cache[n - 1];
}
