/*
 * A375530  a(n) is the denominator of Sum_{k = 1..n} prime(k) / A375529(k).
 * https://oeis.org/A375530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0) = 1, a(n) = prime(n)*a(n-1)^2 + a(n-1) (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Local helper: test a 64‑bit integer for primality (trial division). */
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2ULL;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* ------------------------------------------------------------------ */
/* Dynamic storage for a(·) values                                          */
static seq_t *a_cache = NULL;   /* a(0) … a(current) */
static size_t a_cap  = 0;       /* allocated capacity for a_cache */
static idx_t  a_len  = 0;       /* number of terms stored */

/* ------------------------------------------------------------------ */
/* Dynamic storage for primes                                            */
/* primes[i] holds the (i+1)-th prime (prime(1)=2).  primes_cnt is the
 * number of primes currently stored.  `prime_candidate` remembers the
 * next integer to test for primality. */
static idx_t  *primes          = NULL;
static size_t  primes_cap      = 0;
static size_t  primes_cnt      = 0;
static uint64_t prime_candidate = 2ULL;

/* Ensure a_cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (a_cap > (size_t)n) return;
    size_t new_cap = a_cap ? a_cap << 1 : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_arr) return;  /* ignore allocation failure; program will crash later */
    a_cache = new_arr;
    a_cap   = new_cap;
}

/* Ensure primes array contains at least (n) primes (prime(n) usable). */
static void ensure_primes(idx_t n)
{
    if (primes_cnt > (size_t)n) return;

    /* Grow the primes array if necessary. */
    if (primes_cap < primes_cnt + (size_t)(n - primes_cnt) + 1) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 4;
        while (new_cap < primes_cnt + (size_t)(n - primes_cnt) + 1) new_cap <<= 1;
        idx_t *new_arr = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!new_arr) return;
        primes   = new_arr;
        primes_cap = new_cap;
    }

    /* Generate primes until we have at least (n+1) primes. */
    while (primes_cnt <= (size_t)n) {
        /* Find next prime. */
        while (!is_prime_u64(prime_candidate)) ++prime_candidate;
        /* Store it. */
        primes[primes_cnt] = (idx_t)prime_candidate;
        ++primes_cnt;
        ++prime_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* The required function                                                */
seq_t a(idx_t n)
{
    /* Initialise the cache if necessary. */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        *a_cache = (seq_t)1;  /* a(0) = 1 */
        a_cap   = 1;
        a_len   = 1;
    }

    /* Make sure we have the necessary primes. */
    ensure_primes(n);

    /* Compute missing terms up to n. */
    if (a_len <= (size_t)n) {
        ensure_cache(n);
        for (idx_t i = a_len; i <= n; ++i) {
            if (i == 0) continue;  /* a(0) already stored */
            seq_t prev = a_cache[i - 1];
            seq_t p    = (seq_t)primes[i - 1];  /* prime(i) */
            seq_t next = p * prev * prev + prev;
            a_cache[i] = next;
        }
        a_len = n + 1;
    }

    return a_cache[n];
}
