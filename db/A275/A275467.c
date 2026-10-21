/*
 * A275467  Array read by rows with four columns T(n,k) in which row n lists
 *          the primes > 10 ending in 1, 3, 7, 9 respectively and not yet
 *          present in the sequence.
 * https://oeis.org/A275467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) = smallest prime > a(n‑4) congruent to a(n‑4) mod 10, with
 *            a(1)=11, a(2)=13, a(3)=17, a(4)=19. The function computes terms
 *            on demand and memoizes them. It works for any n≥1
 * Range:     u8 n <= 50, u16 n <= 6491, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

/* static cache of already computed terms */
static seq_t *cache = NULL;         /* array of a(i), 1‑based indexing stored 0‑based */
static size_t cache_size = 0;       /* number of terms currently stored  (>= 0) */
static size_t cache_cap  = 0;       /* current allocated capacity */

seq_t a(idx_t n)
{
    /* Coerce n to size_t for array operations; n is guaranteed ≥1 in usage */
    if (!n) {
        return 0;                    /* undefined, but safe fallback */
    }
    /* Ensure we can store up to the requested index */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        /* double the capacity to amortize realloc cost */
        if (new_cap < 16) new_cap = 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure (unlikely in tests) */
        cache = tmp;
        cache_cap = new_cap;
    }
    /* Compute missing terms up to n */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 4) {
            /* Initial values */
            switch ((int)i) {
                case 1: val = (seq_t)11; break;
                case 2: val = (seq_t)13; break;
                case 3: val = (seq_t)17; break;
                case 4: val = (seq_t)19; break;
                default: val = 0; /* should never happen */
            }
        } else {
            /* a(i) = next prime > a(i-4) with same units digit */
            seq_t prev = cache[i - 5];            /* a(i-4) */
            uint64_t prev_ll = (uint64_t)prev;    /* safe cast for known range */
            uint64_t target_d = prev_ll % 10;     /* required units digit */
            uint64_t cand = next_prime(prev_ll + 1);
            while ((cand % 10) != target_d)
                cand = next_prime(cand + 1);
            val = (seq_t)cand;
        }
        cache[i - 1] = val;
    }
    cache_size = (size_t)n;
    return cache[(size_t)n - 1];
}
