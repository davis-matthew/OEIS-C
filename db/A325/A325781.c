/*
 * A325781  Heinz numbers of complete integer partitions.
 * https://oeis.org/A325781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Heinz numbers of complete integer partitions)
 * Range:     u8 n <= 50, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global data for primes */
static uint64_t *g_primes = NULL;
static size_t   g_prime_cnt = 0;
static uint32_t *g_prime_to_idx = NULL;
static size_t   g_prime_limit = 0;

/* Allocate and compute primes up to a limit */
static void init_primes(void)
{
    /* choose a sufficiently large limit – enough for 10 000‑th term */
    g_prime_limit = 500000;
    bool *is_composite = (bool *)calloc(g_prime_limit + 1, 1);
    if (!is_composite) return;

    /* first count primes for dynamic array allocation */
    for (uint64_t i = 2; i <= g_prime_limit; ++i)
        if (!is_composite[i]) ++g_prime_cnt;

    g_primes = (uint64_t *)malloc(g_prime_cnt * sizeof(uint64_t));
    g_prime_to_idx = (uint32_t *)calloc(g_prime_limit + 1, sizeof(uint32_t));
    if (!g_primes || !g_prime_to_idx) return;

    size_t idx = 0;
    for (uint64_t i = 2; i <= g_prime_limit; ++i) {
        if (!is_composite[i]) {
            g_primes[idx] = i;
            g_prime_to_idx[i] = (uint32_t)(idx + 1); /* prime index starts at 1 */
            ++idx;
        }
        if (i * i <= g_prime_limit) {
            for (uint64_t j = i * i; j <= g_prime_limit; j += i)
                is_composite[j] = true;
        }
    }
    free(is_composite);
}

/* Check if a number n is complete */
static bool is_complete(uint64_t n)
{
    if (n == 1) return true;

    uint64_t temp = n;
    uint64_t idxs[32];
    uint8_t  cnts[32];
    size_t   used = 0;

    /* trial division by primes */
    for (size_t p = 0; p < g_prime_cnt && (uint64_t)g_primes[p] * g_primes[p] <= temp; ++p) {
        uint64_t prime = g_primes[p];
        if (temp % prime == 0) {
            uint8_t c = 0;
            while (temp % prime == 0) {
                temp /= prime;
                ++c;
            }
            idxs[used] = g_prime_to_idx[prime];
            cnts[used] = c;
            ++used;
        }
    }

    if (temp > 1) {          /* remaining prime factor > sqrt(n) */
        idxs[used]   = g_prime_to_idx[temp];
        cnts[used]   = 1;
        ++used;
    }

    /* maximum possible sum of prime indices */
    uint64_t maxSum = 0;
    for (size_t i = 0; i < used; ++i) maxSum += idxs[i] * cnts[i];

    size_t words = (maxSum >> 6) + ((maxSum & 63) ? 1 : 0);
    /* bit arrays */
    uint64_t *cur   = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *next  = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!cur || !next) return false;
    cur[0] = 1ULL;   /* sum 0 reachable */

    for (size_t k = 0; k < used; ++k) {
        uint64_t idx = idxs[k];
        uint8_t  cnt = cnts[k];
        for (uint8_t t = 0; t < cnt; ++t) {
            /* shift cur by idx bits → next */
            uint64_t wshift = idx >> 6;
            uint8_t  bshift = (uint8_t)(idx & 63);
            for (size_t w = words; w-- > wshift;) {
                uint64_t low  = cur[w - wshift] << bshift;
                uint64_t high = 0;
                if (bshift && w - wshift > 0)
                    high = cur[w - wshift - 1] >> (64 - bshift);
                next[w] = low | high;
            }
            /* OR shifted into cur */
            for (size_t i = 0; i < words; ++i) cur[i] |= next[i];
            /* mask any bits beyond maxSum */
            if (maxSum & 63) {
                uint64_t mask = (1ULL << ((maxSum & 63) + 1)) - 1;
                cur[words - 1] &= mask;
            }
            /* clear next for next repetition */
            if (t + 1 < cnt)
                for (size_t i = 0; i < words; ++i) next[i] = 0;
        }
    }

    /* verify that all sums 0..maxSum are reachable */
    bool contiguous = true;
    for (uint64_t s = 0; s <= maxSum; ++s) {
        if (!(cur[s >> 6] & (1ULL << (s & 63)))) {
            contiguous = false;
            break;
        }
    }

    free(cur);
    free(next);
    return contiguous;
}

/* Cache of computed terms */
static seq_t *g_cache   = NULL;
static size_t g_cache_sz = 0;
static size_t g_cache_cap = 0;
static uint64_t g_last_scanned = 1;

/* Ensure cache has enough capacity */
static void ensure_cache(size_t needed)
{
    if (needed <= g_cache_cap) return;
    size_t new_cap = g_cache_cap ? g_cache_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
    if (!new_arr) return;          /* allocation failure – should not happen */
    g_cache = new_arr;
    g_cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!g_cache) {
        init_primes();
        g_cache_cap = 128;
        g_cache = (seq_t *)malloc(g_cache_cap * sizeof(seq_t));
        g_cache_sz = 0;
    }

    while (g_cache_sz < (size_t)n) {
        uint64_t val = g_last_scanned;
        if (is_complete(val)) {
            ensure_cache(g_cache_sz + 1);
            g_cache[g_cache_sz++] = (seq_t)val;
        }
        ++g_last_scanned;
    }
    return g_cache[(size_t)n - 1];
}
