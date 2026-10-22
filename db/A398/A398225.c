/*
 * A398225  Numbers that are neither 5-free nor 5-full.
 * https://oeis.org/A398225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 2305, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A398225: Numbers that are neither 5‑free nor 5‑full. */
/* The sequence is defined by: */
/*    a(n) = the n‑th positive integer that has at least one prime
 *          factor with exponent ≥ 5 and at least one prime factor
 *          with exponent < 5 (i.e. not 5‑free and not 5‑full). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Dynamic smallest‑prime‑factor sieve ---------- */
static uint32_t *spf = NULL;          /* spf[i] == smallest prime factor of i (i≥2) */
static size_t   spf_limit = 0;        /* inclusive limit of spf array */

/* Build spf array up to `limit`.  Caller must guarantee `limit >= 2`. */
static void build_spf(size_t limit)
{
    size_t new_size = limit + 1;
    uint32_t *new_spf = (uint32_t *)realloc(spf, new_size * sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);
    spf = new_spf;
    spf_limit = limit;

    for (size_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }
}

/* Ensure that spf covers values up to `val`.  (Grow by at least a factor.) */
static void ensure_spf(size_t val)
{
    if (val <= spf_limit) return;
    size_t new_lim = (val < 2) ? 2 : val;
    /* Cap growth to avoid extremely large allocations; this is safe for
     * the problem constraints (we need values only up to a few millions). */
    if (new_lim < 1000000) new_lim = 1000000;
    build_spf(new_lim);
}

/* ---------- Property test: neither 5‑free nor 5‑full ---------- */
static bool qualifies(uint64_t n)
{
    if (n < 32) return false;          /* cannot contain a 5th power factor */

    bool has_ge5 = false;
    bool has_lt5 = false;
    uint64_t x = n;

    while (x > 1) {
        uint32_t p = spf[x];
        uint32_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        if (e >= 5) has_ge5 = true;
        else       has_lt5 = true;

        if (has_ge5 && has_lt5) return true;
    }
    return false;
}

/* ---------- Cache of computed sequence terms ---------- */
static seq_t *cache = NULL;     /* cache[0] == a(1) */
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap   = 0;   /* capacity of cache array */

/* Ensure cache has at least `need` capacity. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    cache_cap = new_cap;
}

/* ---------- Main sequence function ---------- */
static uint64_t next_candidate = 2;   /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset 1; value for n==0 is undefined */

    /* Grow cache to at least index n */
    ensure_cache_cap((size_t)n);

    /* Generate terms until we have enough */
    while (cache_size < (size_t)n) {
        /* Ensure sieve covers the candidate */
        if ((size_t)next_candidate > spf_limit)
            ensure_spf((size_t)next_candidate);

        uint64_t val = next_candidate++;
        if (qualifies(val)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = (seq_t)val;
        }
    }

    return cache[n - 1];
}
