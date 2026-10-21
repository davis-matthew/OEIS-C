/*
 * A275770  Primes p == 5 (mod 6) that are not Sophie Germain primes.
 * https://oeis.org/A275770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275770 – primes p ≡ 5 (mod 6) that are NOT Sophie Germain primes.
   Computed by brute‑force search over candidates of the form 6k‑1. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* cached sequence values            */
static size_t cache_cap = 0, cache_sz = 0; /* capacity and current size     */
static size_t next_k = 1;            /* next k such that p = 6*k-1        */

static void ensure_cache(idx_t n)
{
    if (n <= cache_sz) return;

    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return; /* allocation failure */
    }

    while (cache_sz < (size_t)n) {
        uint64_t k = next_k++;
        uint64_t p = 6 * k - 1;                /* candidate prime */
        if (!is_prime((idx_t)p)) continue;    /* not prime, skip */
        uint64_t q = 2 * p + 1;                /* 2p+1 */
        if (is_prime((idx_t)q)) continue;      /* Sophie Germain, skip */

        if (cache_sz == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) return;                 /* allocation failure */
            cache = tmp;
        }
        cache[cache_sz++] = (seq_t)p;          /* store found term */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
