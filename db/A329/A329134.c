/*
 * A329134  Numbers whose differences of prime indices are a periodic word.
 * https://oeis.org/A329134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by enumerating integers and selecting those whose word of
   differences of prime indices is periodic (with at least two differences). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ----- global data for prime tables ----- */
static const idx_t LIMIT = 5000000ULL;          /* upper bound for sieve */
static uint32_t *global_spf        = NULL;     /* smallest prime factor */
static uint32_t *global_prime_index= NULL;     /* 1‑based index of primes, 0 for composites */
static uint32_t *global_primes     = NULL;     /* array of primes */
static size_t   global_primes_cnt  = 0;        /* number of primes */

/* ----- helper: build prime tables ----- */
static void init_prime_tables(void)
{
    if (global_spf != NULL) return;      /* already built */

    global_spf = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    global_prime_index = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    if (!global_spf || !global_prime_index) { exit(1); }

    /* allocate room for primes (number of primes below 5e6 ≈ 348k) */
    global_primes = (uint32_t *)malloc(400000 * sizeof(uint32_t));
    if (!global_primes) { exit(1); }

    global_spf[1] = 1;
    size_t pc = 0;   /* prime counter */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (global_spf[(uint32_t)i] == 0) {
            global_spf[(uint32_t)i] = (uint32_t)i;
            global_prime_index[(uint32_t)i] = (uint32_t)(++pc);
            global_primes[pc - 1] = (uint32_t)i;
        }
        for (size_t j = 0; j < pc && global_primes[j] <= global_spf[(uint32_t)i];
             ++j) {
            idx_t prod = i * (idx_t)global_primes[j];
            if (prod > LIMIT) break;
            global_spf[(uint32_t)prod] = global_primes[j];
        }
    }
    global_primes_cnt = pc;
}

/* ----- test whether number has periodic prime-index‑difference word ----- */
static bool has_periodic_diff(idx_t val)
{
    if (val < 2) return false;   /* 1 and primes have <2 differences */

    unsigned int diff[32];
    unsigned int diff_len = 0;
    unsigned int prev_idx = 0;
    idx_t tmp = val;

    while (tmp > 1) {
        uint32_t p = global_spf[(uint32_t)tmp];
        while ((tmp % p) == 0) {
            unsigned int cur_idx = global_prime_index[p];
            if (prev_idx != 0) {
                diff[diff_len++] = cur_idx - prev_idx;
            }
            prev_idx = cur_idx;
            tmp /= p;
        }
    }

    /* at least two differences needed for periodicity definition */
    if (diff_len < 2) return false;

    /* check for any proper period */
    for (unsigned int d = 1; d < diff_len; ++d) {
        if (diff_len % d != 0) continue;
        bool ok = true;
        for (unsigned int i = 0; i < diff_len; ++i) {
            if (diff[i] != diff[(i + d) % diff_len]) {
                ok = false;
                break;
            }
        }
        if (ok) return true;
    }
    return false;
}

/* ----- cache for sequence values ----- */
static seq_t  *seq_cache   = NULL;
static size_t  cache_len   = 0;
static size_t  cache_cap   = 0;
static idx_t   last_checked = 1;   /* last integer processed */

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for offset 0 */

    /* one‑time initialization */
    static bool initialized = false;
    if (!initialized) {
        init_prime_tables();
        seq_cache = NULL;
        cache_len = 0;
        cache_cap = 0;
        last_checked = 1;
        initialized = true;
    }

    /* return cached value if available */
    if (n <= cache_len) return seq_cache[n - 1];

    /* grow cache as needed */
    if (cache_len >= cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_cache = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
    }

    /* generate numbers until we have the nth term */
    for (;;) {
        ++last_checked;
        if (has_periodic_diff(last_checked)) {
            seq_cache[cache_len++] = (seq_t)last_checked;
            if (cache_len == n) return seq_cache[n - 1];
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap * 2;
                seq_cache = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
                if (!seq_cache) exit(1);
            }
        }
    }
}
