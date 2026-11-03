/*
 * A399925  Numbers k such that A007429(k) is even.
 * https://oeis.org/A399925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9644 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A399925: numbers k such that either v₂(k) is odd or
   v_p(k) ≡ 2 or 3 (mod 4) for some odd prime p.  The function
   generates terms by testing consecutive integers and caches the
   found ones. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether a given k satisfies the condition for A399925. */
static bool k_is_target(idx_t k)
{
    idx_t temp = k;
    int e2 = 0;

    /* remove powers of two */
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e2;
    }
    if (e2 & 1)        /* odd exponent of 2 */
        return true;

    /* factor the odd part and check exponents ≡ 2 or 3 (mod 4) */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if ((e & 3) == 2 || (e & 3) == 3)
                return true;
        }
    }
    /* remaining prime > 1 has exponent 1 (≡ 1 mod 4) → not accepted */
    return false;
}

/* cache for already found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  next_k   = 1;   /* next integer to test */

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    while (cache_len < n) {
        ++next_k;
        if (k_is_target(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
    }
    return cache[n - 1];
}
