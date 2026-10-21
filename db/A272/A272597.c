/*
 * A272597  Numbers n such that the multiplicative group modulo n is the
 *          direct product of 7 cyclic groups.
 * https://oeis.org/A272597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A272597: numbers n such that the multiplicative group
   modulo n is the direct product of 7 cyclic groups (A046072(n) = 7).
   The function returns the n-th such number (1‑based index).
*/

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* static objects for caching the sequence and the spf sieve */
static seq_t *cache     = NULL;          /* array of sequence values   */
static size_t cache_cap = 0;             /* allocated capacity of cache*/
static size_t cache_len = 0;             /* number of terms stored    */
static uint32_t *spf     = NULL;         /* smallest prime factor array */
static uint32_t  spf_lim = 0;            /* current sieve limit       */

/* rebuild the SPF array up to 'limit' (inclusive) */
static void build_spf(uint32_t limit)
{
    if (spf) free(spf);
    spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    for (uint32_t i = 0; i <= limit; ++i) spf[i] = i;
    uint32_t r = (uint32_t)sqrt((double)limit) + 1;
    for (uint32_t i = 2; i <= r; ++i) {
        if (spf[i] == i) {                     /* i is prime */
            uint64_t ii = (uint64_t)i * i;
            for (uint64_t j = ii; j <= limit; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = i;
            }
        }
    }
    spf_lim = limit;
}

/* generate the sequence up to at least 'need' terms */
static void generate_until(idx_t need)
{
    static idx_t start_n = 1;                 /* last tested integer     */
    while (cache_len < (size_t)need) {
        /* increase sieve size if required           */
        if ((uint64_t)start_n >= spf_lim) {
            uint32_t newlim = spf_lim ? spf_lim << 1 : 1 << 20; /* 1M */
            while (newlim <= (uint64_t)start_n) newlim <<= 1;
            build_spf(newlim);
        }

        ++start_n;
        uint64_t n = (uint64_t)start_n;

        /* factor out powers of two */
        uint32_t exp2 = 0;
        while ((n & 1ULL) == 0ULL) { n >>= 1; ++exp2; }

        /* count distinct odd prime factors using SPF */
        int odd_cnt = 0;
        uint32_t last_p = 0;
        while (n > 1) {
            uint32_t p = spf[(uint32_t)n];
            if (p != last_p) { ++odd_cnt; last_p = p; }
            while (n % p == 0) n /= p;
        }

        /* total cyclic factors */
        int total = odd_cnt;
        if (exp2 == 2)          total += 1;
        else if (exp2 >= 3)     total += 2;      /* exp2 == 0 or 1 adds 0 */

        /* store if it satisfies the condition */
        if (total == 7) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 128;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)start_n;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used, but safe */
    generate_until(n);
    return cache[n - 1];
}
