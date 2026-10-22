/*
 * A382967  Biquadratefree numbers (A046100) that are not squarefree
 *          (A005117).
 * https://oeis.org/A382967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate Biquadratefree numbers that are not squarefree.
   A term is accepted if every prime exponent in its factorisation is
   ≤ 3 and at least one exponent is 2 or 3.  The sequence is built on
   demand with a simple cache and trial division using a static sieve
   of primes.  The implementation works for seq_t and idx_t supplied
   by the build and is O(n log n) with a small constant. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- Prime sieve (single initialization) -------------------------------- */
#define PRIME_LIMIT 2000000ULL        /* enough for candidates up to ~10⁶⁶ */
static uint32_t *primes = NULL;      /* array of primes */
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    size_t lim = (size_t)PRIME_LIMIT;
    bool *is_comp = (bool *)malloc((lim + 1) * sizeof(bool));
    if (!is_comp) abort();
    for (size_t i = 0; i <= lim; ++i) is_comp[i] = false;
    for (size_t i = 2; i * i <= lim; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= lim; j += i) is_comp[j] = true;
        }
    }
    size_t pc = 0;
    for (size_t i = 2; i <= lim; ++i) if (!is_comp[i]) ++pc;
    primes = (uint32_t *)malloc(pc * sizeof(uint32_t));
    if (!primes) abort();
    for (size_t i = 2, k = 0; i <= lim; ++i) {
        if (!is_comp[i]) primes[k++] = (uint32_t)i;
    }
    prime_cnt = pc;
    primes_ready = true;
    free(is_comp);
}

/* ---- Test whether n satisfies the defining property --------------------- */
static bool is_biq_free_not_sqfree(uint64_t n)
{
    if (n < 4) return false;          /* the first term is 4 */
    bool has23 = false;
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > m) break;
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
                if (exp > 3) return false; /* exponent too large */
            }
            if (exp >= 2) has23 = true;
        }
    }
    /* Remaining factor (if any) has exponent 1 */
    return has23;
}

/* ---- Cache for computed terms ------------------------------------------- */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static uint64_t next_chk = 2;        /* next integer to examine */

/* ---- Ensure we have at least n terms cached ----------------------------- */
static void ensure_cache(idx_t n)
{
    init_primes();
    while ((idx_t)cache_len < n) {
        uint64_t candidate = next_chk++;
        if (is_biq_free_not_sqfree(candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }
    }
}

/* ---- Sequence entry ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
