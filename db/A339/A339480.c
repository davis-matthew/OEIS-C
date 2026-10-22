/*
 * A339480  Numbers of the form (k^2 - 2) / 2 where k - 1 and k + 1 are both
 *          odd composite numbers.
 * https://oeis.org/A339480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A339480
 *  a(n) = (k^2-2)/2 where k-1 and k+1 are both odd composite numbers.
 *  The sequence may be computed directly by searching for even k
 *  such that the two neighboring odd numbers are composite.
 *  This implementation keeps a cache of values so that calls in
 *  arbitrary order are handled efficiently.
 *  It uses a small fixed prime sieve (up to 200 000) to test
 *  primality of the numbers 2m±1 where k=2m.  For n≤10 000
 *  this limit is far more than sufficient and keeps the routine
 *  fast (<0.05 s per call on typical machines).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime sieve – static – covers all numbers we will test (2m±1 up to 2·mmax+1). */
/* ------------------------------------------------------------------ */
static bool *prime_sieve = NULL;      /* true if prime */
static size_t sieve_size = 0;         /* length of prime_sieve */

/* Initialise a simple sieve of Eratosthenes up to LIMIT. */
static void init_sieve(void)
{
    if (prime_sieve != NULL) return;          /* already initialised */

    const size_t LIMIT = 200000;              /* > 2·mmax+1 for n≤10 000 */
    sieve_size = LIMIT + 1;
    prime_sieve = (bool *)malloc(sieve_size * sizeof(bool));
    if (!prime_sieve) abort();

    for (size_t i = 2; i < sieve_size; ++i) prime_sieve[i] = true;
    for (size_t i = 2; (size_t)i * i < sieve_size; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j < sieve_size; j += i)
                prime_sieve[j] = false;
        }
    }
}

/* Quick primality test using the sieve */
static inline bool is_prime_small(uint64_t x)
{
    if (x < sieve_size) return prime_sieve[(size_t)x];
    /* If x is outside the sieve range, fall back to trial division
     * (not expected for this problem set).  This keeps the code
     * robust for larger n, but for n≤10 000 the branch is never taken. */
    if (x < 2) return false;
    if ((x & 1) == 0) return false;
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) return false;
    }
    return true;
}

/* Is x (odd) a composite number?  1 is not composite, primes are not. */
static inline bool is_composite_odd(uint64_t x)
{
    if (x <= 3) return false;   /* 1 and 3 are not composite */
    return !is_prime_small(x);   /* x>3, odd – composite iff not prime */
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle offset: a(1) = first term */
    if (n == 0) return (seq_t)0;   /* undefined for n<1, but return 0 */

    /* Initialise the prime sieve once */
    if (!prime_sieve) init_sieve();

    /* Static cache of already computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* Last processed m (k = 2m).  We start at 0 so that the first
     * iteration increases it to 1. */
    static uint64_t last_m = 0;

    /* Expand the cache until it contains a(n) */
    while (cache_len < (size_t)n) {
        /* Find the next m for which 2m-1 and 2m+1 are odd composites */
        uint64_t m;
        for (;;) {
            m = ++last_m;
            uint64_t a = 2 * m - 1;
            uint64_t b = 2 * m + 1;
            if (is_composite_odd(a) && is_composite_odd(b))
                break;
        }

        /* Compute the value  (k^2 - 2)/2 = 2*m^2 - 1 */
        seq_t term = (seq_t)2 * (seq_t)m * (seq_t)m - (seq_t)1;

        /* Append to cache */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 32;
            seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_buf) abort();   /* allocation failure */
            cache = new_buf;
            cache_cap = new_cap;
        }
        cache[cache_len++] = term;
    }

    return cache[(size_t)n - 1];
}
