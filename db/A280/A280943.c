/*
 * A280943  Least number k such that sopfr(k) = sopfr(k + n), where sopfr(k)
 *          is the integer log of k.
 * https://oeis.org/A280943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the least k such that sopfr(k) = sopfr(k + n), where
 *            sopfr(m) = sum of prime factors of m (with multiplicity)
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Cached values of a(n) (index 0 is unused; a(1) is the first term) */
static seq_t *cache = NULL;
static size_t cache_len = 0;

static void cache_extend(idx_t n)
{
    if ((size_t)n < cache_len) return;
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) abort();
    for (size_t i = cache_len; i < new_len; ++i) new_cache[i] = 0; /* 0 = not computed */
    cache = new_cache;
    cache_len = new_len;
}

/* Smallest prime factor array (indexed 0..limit). 0 and 1 have value 0. */
static uint32_t *spf = NULL;
static size_t spf_size = 0;

/* Build a SPF table up to 'limit' (inclusive).  Re‑builds from scratch
   every time the table must be enlarged. */
static void ensure_spf(size_t limit)
{
    if (limit < 2) limit = 2;
    if (spf_size > 0 && spf_size-1 >= limit) return;

    free(spf);
    spf_size = limit + 1;
    spf = (uint32_t*)calloc(spf_size, sizeof(uint32_t));
    if (!spf) abort();

    for (size_t i = 2; i < spf_size; ++i) spf[i] = (uint32_t)i;

    size_t sqrt_lim = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= sqrt_lim; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j < spf_size; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* Sum of prime factors (with multiplicity) for a 64‑bit integer.
   Assumes spf table covers the argument. */
static seq_t calc_sopfr_uint64(uint64_t x)
{
    if (x < 2) return (seq_t)0;
    seq_t sum = 0;
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        sum += (seq_t)p;
        x /= (uint64_t)p;
    }
    return sum;
}

/* Main sequence function.  The cache avoids recomputation of the same n. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* never used in OEIS data, safe fallback */

    cache_extend(n);
    if (cache[n] != 0) return cache[n];

    uint64_t n64 = (uint64_t)n;
    uint64_t k = 1;
    for (;;) {
        uint64_t max_needed = k + n64;
        ensure_spf(max_needed);

        seq_t s1 = calc_sopfr_uint64(k);
        seq_t s2 = calc_sopfr_uint64(k + n64);
        if (s1 == s2) {
            cache[n] = (seq_t)k;
            return (seq_t)k;
        }
        ++k;
    }
}
