/*
 * A398164  Numbers k such that both k and k+1 are both exponentially
 *          squarefree numbers.
 * https://oeis.org/A398164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 235, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp_is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers k such that both k and k+1 are exponentially squarefree */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/exp_is_squarefree.h"

/* ---------- Prime sieve ---------- */
static uint32_t *prime_list = NULL;
static size_t      prime_cnt  = 0;
static bool        primes_ready = false;

static void prepare_primes(void)
{
    if (primes_ready) return;
    const idx_t LIMIT = 2000000;   /* sufficient for all tested n */
    bool *mark = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!mark) return;             /* memory failure – unlikely */

    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (!mark[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                mark[j] = true;

    /* count primes */
    size_t c = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!mark[i]) ++c;

    prime_list = (uint32_t*)calloc(c, sizeof(uint32_t));
    if (!prime_list) { free(mark); return; }
    size_t pos = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!mark[i]) prime_list[pos++] = (uint32_t)i;

    prime_cnt = c;
    free(mark);
    primes_ready = true;
}

/* ---------- test if a number is exponentially squarefree ---------- */
static bool exp_squarefree(uint64_t n)
{
    if (n <= 1) return true;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }
            if (!exp_is_squarefree(exp)) return false;
        }
    }
    /* remaining factor >1 has exponent 1 -> always squarefree */
    return true;
}

/* ---------- cache of sequence values ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static unsigned long long next_k = 1;

static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, safe fallback */
    prepare_primes();

    while (cache_len < (size_t)n) {
        uint64_t k = (uint64_t)next_k;
        if (exp_squarefree(k) && exp_squarefree(k + 1)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
