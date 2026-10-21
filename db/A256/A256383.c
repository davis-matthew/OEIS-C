/*
 * A256383  Numbers n such that n-5 and n+5 are semiprimes.
 * https://oeis.org/A256383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n such that n‑5 and n+5 are semiprimes (product of two distinct primes) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- Prime factor sieve for fast semiprime test ----- */
static uint32_t *spf = NULL;          /* smallest prime factor array */
static uint32_t  spf_limit = 0;        /* highest index currently covered */

/* Build sieve of Eratosthenes up to 'limit', storing smallest prime factors. */
static void build_spf(uint32_t limit)
{
    uint32_t *new_spf = (uint32_t*)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!new_spf) abort();

    for (uint32_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {            /* i is prime */
            new_spf[i] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i) {
                    if (new_spf[(size_t)j] == 0)
                        new_spf[(size_t)j] = i;
                }
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* Ensure the sieve covers at least 'needed'. Rebuild if necessary. */
static void ensure_spf(uint32_t needed)
{
    if (needed <= spf_limit) return;
    uint32_t new_limit = needed;
    build_spf(new_limit);
}

/* Check whether x (x>=4) is a semiprime: product of two primes. */
static bool is_semiprime(uint32_t x)
{
    if (x < 4 || x > spf_limit) return false;
    uint32_t p = spf[x];
    if (p == 0 || p == x) return false;   /* prime or uninitialized */
    uint32_t q = x / p;
    if (q == 1) return false;
    return spf[q] == q; /* q is prime */
}

/* ----- Sequence storage ----- */
static seq_t *seq_cache   = NULL;   /* dynamic array of found terms */
static size_t seq_cap    = 0;       /* allocated capacity */
static size_t seq_len    = 0;       /* number of terms stored */
static idx_t  next_check = 1;       /* next integer to test as candidate */

/* Ensure seq_cache can hold 'needed' elements. */
static void enlarge_cache(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_buf = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_buf) abort();
    seq_cache = new_buf;
    seq_cap = new_cap;
}

/* Append a term to seq_cache. */
static void append_term(seq_t val)
{
    enlarge_cache(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) abort();      /* offset is 1; caller guarantees n>=1 */

    /* First call: initialise buffers and sieve. */
    if (seq_cache == NULL) {
        seq_cache = (seq_t*)malloc(64 * sizeof(seq_t));
        if (!seq_cache) abort();
        seq_cap = 64;
        seq_len = 0;
        next_check = 1;
        /* Seed the sieve with a modest initial limit; will grow on demand. */
        build_spf(2000000U);    /* ample for n up to ~10,000 */
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        uint32_t m = (uint32_t)next_check;        /* candidate number */

        /* Ensure sieve covers m+5. */
        ensure_spf(m + 5U);

        if (is_semiprime(m - 5U) && is_semiprime(m + 5U)) {
            append_term((seq_t)m);
        }
        ++next_check;
    }

    return seq_cache[(size_t)n - 1];
}
