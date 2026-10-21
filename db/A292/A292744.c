/*
 * A292744  a(0) = 1; a(n) = Sum_{k=1..n} prime(k+1)*a(n-k).
 * https://oeis.org/A292744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} prime(k+1) * a(n-k) (OEIS A292744)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime generation (trial‑division, small enough for n <= 10000) */
static uint64_t *prime_list = NULL;        /* 0‑based: prime_list[0] = 2 */
static size_t   prime_cap   = 0;
static size_t   prime_cnt   = 0;

static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;

    if (prime_cap == 0) {
        prime_cap = 64;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) abort();
    }

    if (prime_cnt == 0) {               /* insert first prime */
        prime_list[prime_cnt++] = 2;
    }

    uint64_t cand = prime_list[prime_cnt-1] + 1;
    while (prime_cnt < need) {
        bool prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) {
            if (prime_cap == prime_cnt) {
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc(prime_list,
                                                 prime_cap * sizeof(uint64_t));
                if (!prime_list) abort();
            }
            prime_list[prime_cnt++] = cand;
        }
        ++cand;
    }
}

/* Sequence cache */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!seq_cache) abort();
        for (size_t i = cache_cap; i < new_cap; ++i) seq_cache[i] = 0;
        cache_cap = new_cap;
    }
    if (seq_cache[0] == 0) seq_cache[0] = 1;
    ensure_primes((size_t)n + 1);
}

/* Public function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (seq_cache[n] != 0) return seq_cache[n];
    if (n == 0) return seq_cache[0] = 1;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t prime = (seq_t)prime_list[(size_t)k];          /* prime(k+1)  */
        seq_t term  = a(n - k);                             /* recursive memo */
        /* Use 128‑bit intermediate to avoid 64‑bit overflow when needed. */
        sum += (seq_t)(((__uint128_t)prime) * term);
    }
    seq_cache[n] = sum;
    return sum;
}
