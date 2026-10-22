/*
 * A302797  Squarefree numbers whose prime indices are pairwise coprime. Heinz
 *          numbers of strict integer partitions with pairwise coprime parts.
 * https://oeis.org/A302797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sieve-based search for square‑free numbers whose prime
   indices are pairwise coprime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Sieve range – sufficient for the first 10 000 terms of the sequence. */
static const uint32_t SIEVE_LIMIT = 10000000U;

/* Linear sieve data structures. */
static uint32_t *spf_arr = NULL;          /* smallest prime factor */
static uint32_t *prime_index_arr = NULL;  /* 1‑based index of prime, 0 if composite */
static uint32_t *prime_list = NULL;       /* list of primes */
static size_t prime_count = 0;
static bool sieve_ready = false;

/* Greatest common divisor for 32‑bit values. */
static inline uint32_t gcd_uint32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Build the linear sieve up to SIEVE_LIMIT. */
static void build_sieve(void)
{
    if (sieve_ready) return;
    size_t n = SIEVE_LIMIT + 1;

    spf_arr          = (uint32_t *)malloc(n * sizeof(uint32_t));
    prime_index_arr  = (uint32_t *)malloc(n * sizeof(uint32_t));
    prime_list       = (uint32_t *)malloc(n * sizeof(uint32_t));
    if (!spf_arr || !prime_index_arr || !prime_list) exit(EXIT_FAILURE);

    for (size_t i = 0; i < n; ++i) {
        spf_arr[i] = 0;
        prime_index_arr[i] = 0;
    }

    for (size_t i = 2; i < n; ++i) {
        if (spf_arr[i] == 0) {
            spf_arr[i] = (uint32_t)i;
            prime_index_arr[i] = (uint32_t)(prime_count + 1);  /* 1‑based index */
            prime_list[prime_count] = (uint32_t)i;
            ++prime_count;
        }
        for (size_t j = 0; j < prime_count; ++j) {
            uint64_t prod = (uint64_t)i * prime_list[j];
            if (prod >= n) break;
            spf_arr[prod] = prime_list[j];
            if (i % prime_list[j] == 0) break;
        }
    }
    sieve_ready = true;
}

/* Check if a number is squarefree and its prime indices are pairwise coprime. */
static bool is_valid(seq_t m)
{
    if (m == (seq_t)1) return true;           /* empty set → 1 */

    if (m > (seq_t)SIEVE_LIMIT) return false; /* outside sieve range */

    uint32_t x = (uint32_t)m;
    uint32_t last_p = 0;
    uint32_t indices[64];
    uint32_t idx_cnt = 0;

    while (x > 1) {
        uint32_t p = spf_arr[x];
        if (p == 0) p = x;               /* safety for numbers > SIEVE_LIMIT */

        /* count exponent – must be 1 for squarefree */
        uint32_t exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
            if (exp > 1) return false;   /* not squarefree */
        }

        indices[idx_cnt++] = prime_index_arr[p];
        last_p = p;
    }

    if (idx_cnt == 1) {
        return indices[0] == 1;          /* only prime 2 allowed as single factor */
    }

    for (uint32_t i = 0; i < idx_cnt; ++i) {
        for (uint32_t j = i + 1; j < idx_cnt; ++j) {
            if (gcd_uint32(indices[i], indices[j]) != 1) return false;
        }
    }
    return true;
}

/* Cache of computed terms. */
static seq_t *term_cache = NULL;
static size_t cache_cap = 0;
static size_t term_count = 0;
static uint32_t last_checked = 0;

/* Sequence entry. */
seq_t a(idx_t n)
{
    if (!sieve_ready) build_sieve();

    if (!term_cache) {
        cache_cap = 128;
        term_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!term_cache) exit(EXIT_FAILURE);
    }

    while (term_count < (size_t)n) {
        ++last_checked;
        if (last_checked > SIEVE_LIMIT) exit(EXIT_FAILURE);

        if (is_valid((seq_t)last_checked)) {
            if (term_count == cache_cap) {
                cache_cap <<= 1;
                term_cache = (seq_t *)realloc(term_cache, cache_cap * sizeof(seq_t));
                if (!term_cache) exit(EXIT_FAILURE);
            }
            term_cache[term_count++] = (seq_t)last_checked;
        }
    }
    return term_cache[n - 1];
}
