/*
 * A320159  Number of positive integers x < prime(n)/2 such that (x^2 mod
 *          prime(n)) > (4*x^2 mod prime(n)).
 * https://oeis.org/A320159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive integers x < prime(n)/2 such that *
 *            (x^2 mod prime(n)) > (4*x^2 mod prime(n))
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* ============================= Prime generation ============================= */
/* Stand‑alone implementation of the nth prime (1‑based) using incremental
 * trial division. This does not rely on any external headers that may be
 * missing in the build environment. It's fast enough for prime(10000)≈104729. */
static uint32_t *primes = NULL;       /* array of found primes */
static size_t  prime_cap  = 0;        /* allocated capacity of primes */
static size_t  prime_cnt  = 0;        /* number of primes stored      */
static uint32_t last_candidate = 1;   /* last tested candidate (>=1)   */

static void prime_grow(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) exit(1); /* out of memory */
    primes = tmp;
    prime_cap = new_cap;
}

/* Ensure that at least 'need' primes are stored. */
static void prime_ensure(size_t need)
{
    while (prime_cnt < need) {
        uint32_t cand = last_candidate + 1;
        bool ok;
        do {
            ok = true;
            uint32_t r = (uint32_t)sqrt((double)cand);
            for (size_t i = 0; i < prime_cnt && primes[i] <= r; ++i) {
                if (cand % primes[i] == 0) { ok = false; break; }
            }
            if (!ok) ++cand;
        } while (!ok);

        if (prime_cnt == prime_cap) prime_grow();
        primes[prime_cnt++] = cand;
        last_candidate = cand;
    }
}

/* Retrieve the nth prime (1‑based). */
static uint32_t get_prime(size_t n)
{
    if (n == 0) return 0;
    prime_ensure(n);
    return primes[n - 1];
}

/* ============================= Sequence computation ============================= */
/* Cache of previously computed sequence values */
static seq_t *cache_vals = NULL;
static size_t  cache_cap  = 0; /* allocated size of cache_vals */
static size_t  cache_cnt  = 0; /* number of cached values */

/* Grow cache as needed */
static void cache_grow(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache_vals = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) and cache intermediate results as needed. */
seq_t a(idx_t n)
{
    /* Validate the index */
    if (n == 0) return 0;

    /* Ensure we have the n‑th prime. */
    uint32_t p = get_prime((size_t)n);

    /* Extend cache up to n. */
    while (cache_cnt < (size_t)n) {
        /* Compute the value for current index (cache_cnt + 1). */
        size_t idx = cache_cnt;          /* zero‑based index for new value */
        uint32_t prime = get_prime(idx + 1);  /* nth prime */

        /* Upper bound for x: integer x < p/2.  For prime=2 we have none. */
        uint64_t max_x = (uint64_t)(prime - 1) / 2;

        seq_t count = 0;
        uint64_t y = 0;            /* current x^2 mod p */

        for (uint64_t x = 1; x <= max_x; ++x) {
            /* Update y = x^2 mod p using recurrence y = prev + (2x - 1) mod p */
            y += (2 * x - 1);
            if (y >= prime) y -= prime;  /* 2x-1 < 2p, so one subtraction suffices */

            /* t = 4*y mod p ; 4*y < 4p -> up to 3 subtractions */
            uint64_t t = y << 2;
            if (t >= prime) {
                t -= prime;
                if (t >= prime) {
                    t -= prime;
                    if (t >= prime) t -= prime;
                }
            }

            if (y > t) ++count;
        }

        /* Store result */
        if (cache_cnt == cache_cap) cache_grow();
        cache_vals[cache_cnt++] = count;
    }

    return cache_vals[n - 1];
}
