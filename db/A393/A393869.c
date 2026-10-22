/*
 * A393869  Numbers k such that A053669(k) < A003557(k).
 * https://oeis.org/A393869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th integer k such that * q(k) < k / rad(k), *
 *            where q(k) is the smallest prime not dividing k * and rad(k) is
 *            the product of distinct prime divisors of k
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a static sieve of smallest prime factors up to
 * a fixed limit (2'000'000).  The sequence values are generated on demand
 * and cached in a dynamically resized array.  The code is valid C11 and
 * C++17. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- static state ------------------------------------------- */
static idx_t spf_limit = 0;          /* current size of the spf array   */
static uint32_t *spf = NULL;         /* lowest prime factor of i        */
static uint32_t *primes = NULL;      /* list of primes up to spf_limit */
static size_t primes_count = 0;      /* number of primes               */

static seq_t *cache = NULL;          /* cached a(n) values              */
static size_t cache_len = 0;         /* number of cached elements       */
static size_t cache_cap = 0;         /* capacity of cache               */

static idx_t next_k = 2;             /* next integer to test           */

static bool init_done = false;

/* --------- helper functions --------------------------------------- */
static void build_spf(idx_t limit)
{
    /* allocate and compute SPF array for [0..limit] */
    uint32_t *new_spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!new_spf) return;                 /* allocation failure  */
    new_spf[0] = 0;
    new_spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if ((idx_t)i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = (uint32_t)i;
            }
        }
    }
    /* free old array if present */
    if (spf) free(spf);
    spf = new_spf;
    spf_limit = limit;
}

static void build_primes(void)
{
    /* count and store all primes up to spf_limit */
    size_t cnt = 0;
    for (idx_t i = 2; i <= spf_limit; ++i)
        if (spf[i] == i)
            ++cnt;
    primes_count = cnt;
    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!primes) return;
    size_t idx = 0;
    for (idx_t i = 2; i <= spf_limit; ++i)
        if (spf[i] == i)
            primes[idx++] = (uint32_t)i;
}

static void init(void)
{
    if (init_done) return;
    const idx_t LIMIT = 2000000;          /* should cover many terms  */
    build_spf(LIMIT);
    build_primes();
    cache_cap = 16;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache_len = 0;
    init_done = true;
}

static void cache_append(seq_t v)
{
    if (cache_len >= cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = v;
}

/* --------- public function ---------------------------------------- */
seq_t a(idx_t n)
{
    init();
    while (cache_len < n) {
        /* factorise next_k using SPF array */
        idx_t k = next_k;
        idx_t temp = k;
        seq_t rad = 1;          /* product of distinct primes   */
        seq_t m   = 1;          /* k / rad(k)                   */
        bool has_repeat = false;

        while (temp > 1) {
            uint32_t p = spf[temp];
            uint32_t exp = 0;
            while ((temp % p) == 0) {
                temp /= p;
                ++exp;
            }
            rad *= (seq_t)p;
            if (exp > 1) {
                has_repeat = true;
                seq_t powp = 1;
                for (uint32_t i = 1; i < exp; ++i)
                    powp *= (seq_t)p;
                m *= powp;
            }
        }

        if (has_repeat) {
            /* find smallest prime not dividing k */
            uint32_t q = 0;
            for (size_t i = 0; i < primes_count; ++i) {
                uint32_t p = primes[i];
                if ((k % p) != 0) {
                    q = p;
                    break;
                }
            }
            /* All primes up to spf_limit divide k?  This cannot happen
             * for k <= spf_limit because the product would exceed k.*/
            if (q != 0 && (seq_t)q < m) {
                cache_append((seq_t)k);
            }
        }
        ++next_k;
    }
    return cache[n - 1];
}
