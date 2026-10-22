/*
 * A355965  Primes p such that (p+10)^(p-1) == 1 (mod p^2).
 * https://oeis.org/A355965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   mod_pow, next_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p such that (p+10)^(p-1) ≡ 1 (mod p²).
   The function returns the n‑th such prime (a(1)=13).
   It searches primes incrementally, using modular exponentiation
   to test the condition. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"
#include "../utils/next_prime.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* Next prime to test */
    static uint64_t next_start = 2;
    /* Lazy initialization */
    if (cache == NULL) {
        cache_cap = 16;
        cache = seq_alloc(cache_cap);
    }

    /* Keep searching until we have n terms */
    while (cache_len < (size_t)n) {
        uint64_t p = next_prime(next_start);      /* prime to test */
        seq_t base = (seq_t)p + (seq_t)10;
        seq_t mod  = (seq_t)p * (seq_t)p;
        seq_t r    = mod_pow(base, (uint64_t)p - 1, mod);
        if (r == 1) {  /* p satisfies the condition */
            if (cache_len == cache_cap) { /* grow cache */
                size_t new_cap = cache_cap * 2;
                if (new_cap == 0) new_cap = 1;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
        next_start = p + 1;   /* next search starts after current prime */
    }
    return cache[n - 1];
}
