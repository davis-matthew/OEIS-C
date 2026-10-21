/*
 * A243441  Primes p such that p + A000120(p) is also a prime, where A000120 =
 *          sum of digits in base 2 = Hamming weight.
 * https://oeis.org/A243441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 565, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A243441
 *
 * a(n) = the n-th prime p such that p + popcount(p) is also prime.
 * popcount(p) is the number of 1‑bits in the binary representation of p.
 *
 * This implementation builds primes up to a dynamically chosen limit,
 * collects all qualifying primes, and stores them in a static
 * cache. Subsequent calls reuse the cache or extend it if the requested
 * index is larger than the current cache size. The algorithm is
 * straightforward and runs in well under the 2‑second limit for
 * n up to about 10 000.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Popcount for a uint64_t (idx_t is typedef’ed as uint64_t by the build). */
static unsigned popcount_u64(idx_t x)
{
    unsigned c = 0;
    while (x) {
        c += (unsigned)(x & 1ULL);
        x >>= 1;
    }
    return c;
}

/* Global caches for the sequence. */
static idx_t *seq_cache = NULL;   /* array of qualifying primes (idx_t) */
static size_t  seq_len   = 0;     /* number of cached terms          */
static bool    seq_ready = false;/* set when cache is fully built    */

/* Free the current cache. */
static void free_cache(void)
{
    free(seq_cache);
    seq_cache = NULL;
    seq_len   = 0;
}

/* Build the cache up to the given upper bound LIMIT (inclusive). */
static void build_cache(idx_t LIMIT)
{
    /* Allocate prime sieve array. */
    char *prime = (char*)malloc((size_t)(LIMIT + 1));
    if (!prime) return;              /* allocation failure – silently ignore */

    /* Initialize sieve. */
    prime[0] = prime[1] = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) prime[i] = 1;

    /* Sieve of Eratosthenes. */
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (!prime[i]) continue;
        for (idx_t j = i * i; j <= LIMIT; j += i) prime[j] = 0;
    }

    /* Reserve space for qualifying primes. */
    size_t cap = (size_t)(LIMIT / 10 + 100);   /* rough estimate */
    seq_cache = (idx_t*)malloc(cap * sizeof(idx_t));
    if (!seq_cache) { free(prime); return; }
    seq_len = 0;

    /* Scan primes and collect those satisfying the condition. */
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (!prime[p]) continue;
        idx_t pc = (idx_t)popcount_u64(p);
        idx_t q  = p + pc;
        if (q <= LIMIT && prime[q]) {
            if (seq_len == cap) {                 /* grow the array */
                cap *= 2;
                idx_t *tmp = (idx_t*)realloc(seq_cache, cap * sizeof(idx_t));
                if (!tmp) break;
                seq_cache = tmp;
            }
            seq_cache[seq_len++] = p;
        }
    }

    free(prime);
}

/* Ensure that the cache contains at least n terms.  The function grows
 * the sieve upper bound exponentially until enough terms are found. */
static void ensure_cache(idx_t n)
{
    if (seq_ready && seq_len >= n) return;

    /* Start with a reasonable bound; double until sufficient. */
    idx_t limit = 2000000ULL;          /* sufficient for n up to ~10 000 */
    while (true) {
        free_cache();
        build_cache(limit);
        if (seq_len >= n) { seq_ready = true; return; }
        limit *= 2;                    /* increase bound */
        if (limit > 100000000ULL) {    /* safety cap – unlikely needed */
            seq_ready = true;
            return;
        }
    }
}

/* Public function demanded by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined – defensive */
    ensure_cache(n);
    if (n - 1 < seq_len) return (seq_t)seq_cache[n - 1];
    return 0;                            /* fallback – out of range */
}
