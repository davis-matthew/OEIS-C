/*
 * A332226  Numbers k such that sigma(k) is congruent to 2 modulo 8.
 * https://oeis.org/A332226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that sigma(k) ≡ 2 (mod 8)
 * Range:     u8 n <= 34, u16 n <= 4782, u32 n <= 9805, u64 n <= 9805,
 *            u128 n <= 9805, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External types: seq_t is the sequence element type, idx_t the index type */
static seq_t *cache      = NULL;    /* Cached sequence values */
static idx_t  cache_len  = 0;       /* Number of cached terms */
static size_t cache_cap  = 0;       /* Allocated capacity of cache */

static uint32_t *spf      = NULL;    /* Smallest prime factor table */
static uint64_t  spf_max  = 0;       /* Largest number for which spf is valid */

static uint64_t last_checked = 1;    /* Last integer examined */

/* Grow the cache array to hold at least 'needed' elements */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Extend the spf array to support numbers up to 'need' */
static void ensure_spf(size_t need)
{
    if (need <= spf_max) return;
    size_t new_limit = spf_max ? spf_max * 2 : 1024;
    while (new_limit < need) new_limit *= 2;

    uint32_t *new_spf = (uint32_t *)malloc((new_limit + 1) * sizeof(uint32_t));
    if (!new_spf) abort();
    for (size_t i = 0; i <= new_limit; ++i) new_spf[i] = 0;

    for (size_t i = 2; i <= new_limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= new_limit) {
                for (uint64_t j = (uint64_t)i * i; j <= new_limit; j += i)
                    if (new_spf[(size_t)j] == 0) new_spf[(size_t)j] = (uint32_t)i;
            }
        }
    }
    free(spf);
    spf = new_spf;
    spf_max = new_limit;
}

/* Compute sigma(n) modulo 8 using the spf table */
static int sigma_mod8(uint64_t n)
{
    if (n == 1) return 1;          /* sigma(1) = 1 */
    int result = 1;
    uint64_t temp = n;
    while (temp > 1) {
        uint32_t p = spf[(size_t)temp];
        int sum   = 1;            /* p^0 contributes 1 */
        int pow   = 1;            /* current power of p modulo 8 */
        while (temp % p == 0) {
            temp /= p;
            pow = (pow * (p % 8)) % 8;
            sum = (sum + pow) % 8;
        }
        result = (result * sum) % 8;
    }
    return result;
}

/* Return the n‑th term of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* Undefined – return 0 for safety */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        ++last_checked;
        ensure_spf((size_t)last_checked);
        if (sigma_mod8(last_checked) == 2) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
