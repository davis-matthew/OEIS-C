/*
 * A387711  Numbers k for which A003959(k) > 2*k, where A003959 is
 *          multiplicative with a(p^e) = (p+1)^e.
 * https://oeis.org/A387711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2339, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A387711: the n‑th integer k for which
   A003959(k) > 2·k, where A003959(k)=∏(p+1)^e for k=∏p^e.
   The function uses a sieve for smallest prime factors and
   builds the list of qualifying k's lazily, caching the results
   for future calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Parameters -------------------------------------------------------- */

#define SIEVE_LIMIT (10000000ULL)   /* should cover enough k for n≤10000 */

/* ------------------------------------------------------------------ */
/* State ------------------------------------------------------------- */

static uint32_t *spf = NULL;          /* smallest prime factor table   */
static bool     spf_initialized = false;  /* one‑time initialization flag  */

static seq_t   *cache = NULL;          /* cached good k's              */
static size_t  cache_len = 0;          /* number of cached terms       */
static size_t  cache_cap = 0;          /* capacity of cache vector     */
static idx_t   next_k = 2;             /* next integer to test         */

/* ------------------------------------------------------------------ */
/* Helper: initialize the smallest prime factor sieve.                */

static void init_spf(void)
{
    if (spf_initialized) return;

    /* Allocate SPF array.  Use C style cast to satisfy both C and C++ */
    spf = (uint32_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    if (!spf) abort();  /* out of memory */

    /* Sieve of Eratosthenes to fill spf array */
    for (uint64_t i = 0; i <= SIEVE_LIMIT; ++i) spf[i] = (uint32_t)i;
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
        if (spf[i] != i) continue;          /* i not prime */
        for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i) {
            if (spf[(size_t)j] == j) spf[(size_t)j] = i;  /* set first prime factor */
        }
    }
    spf_initialized = true;
}

/* ------------------------------------------------------------------ */
/* Test whether a given k satisfies A003959(k) > 2·k.                  */

static bool good_k(idx_t k)
{
    seq_t prod = (seq_t)1;
    seq_t threshold = ((seq_t)2) * ((seq_t)k);

    idx_t rem = k;
    while (rem > 1) {
        uint32_t p = spf[(size_t)rem];
        uint32_t e = 0;
        while (rem % p == 0) {
            rem /= p;
            ++e;
        }
        seq_t base = (seq_t)p + 1;
        for (uint32_t i = 0; i < e; ++i) {
            prod *= base;
            if (prod > threshold) return true;  /* early exit */
        }
    }
    return prod > threshold;
}

/* ------------------------------------------------------------------ */
/* Append a value to the cache, resizing if necessary.                */

static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                    /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Public sequence function. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 */

    init_spf();

    /* Build cache up to the required index n */
    while (cache_len < (size_t)n) {
        if (good_k(next_k)) {
            cache_append((seq_t)next_k);
        }
        ++next_k;
    }
    return cache[n - 1];
}
