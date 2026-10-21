/*
 * A268375  Numbers k for which A001222(k) = A267116(k).
 * https://oeis.org/A268375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268375: a(n) is the n‑th positive integer whose prime‑factor
   exponents have pairwise disjoint binary representations.
   The function builds the sequence by testing consecutive integers and
   collecting those that satisfy the condition. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* The types idx_t (uint64_t) and seq_t (unsigned integer of chosen width)
   are defined by the build environment; they are not redeclared here. */

/* ---------- Prime sieve for factorisation ----------
   The sieve is built once on the first call to a().  The chosen limit
   (2 million) is sufficient for n ≤ 10 000. */
static uint32_t *primes     = NULL;
static size_t   primes_cnt  = 0;
static size_t   sieve_limit = 0;

static void init_primes(void)
{
    if (primes != NULL) return;            /* already initialised */

    const size_t LIMIT = 2000000;          /* 2×10⁶ is ample */

    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) abort();

    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++cnt;

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) abort();
    primes_cnt = cnt;

    size_t pos = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) primes[pos++] = (uint32_t)i;
    }

    free(is_prime);
    sieve_limit = LIMIT;
}

/* ---------- Test whether a number satisfies the condition ----------
   For each prime factor we extract its exponent e; the bits set in e
   must be disjoint from those of all previous exponents. */
static bool satisfies_property(uint64_t n)
{
    if (n == 1) return true;        /* 1 has no prime factors */

    uint64_t used = 0;
    uint64_t t = n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > t) break;
        if (t % p == 0) {
            unsigned int exp = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            if (used & ((uint64_t)exp)) return false;
            used |= (uint64_t)exp;
        }
    }

    /* Remaining prime factor, exponent 1 */
    if (t > 1) {
        if (used & 1ULL) return false;
        used |= 1ULL;
    }

    return true;
}

/* ---------- Cache of already found sequence terms ----------
   cache[i] holds a(i+1). */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static uint64_t checked = 1;   /* next integer to test */

/* Append a value to the cache, reallocating as needed. */
static void cache_append(seq_t val)
{
    static size_t cache_cap = 0;
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 4 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Generate terms until we have at least n values cached. */
static void generate_until(idx_t n)
{
    while (cache_len < (size_t)n) {
        if (satisfies_property(checked))
            cache_append((seq_t)checked);
        ++checked;
    }
}

/* ---------- Public function required by the build ---------- */
seq_t a(idx_t n)
{
    init_primes();

    if (n == 0) return 0;          /* behavior below the offset is irrelevant */

    generate_until(n);
    return cache[n - 1];
}
