/*
 * A353072  Numbers k such that nextprime(k)-k is a positive square.
 * https://oeis.org/A353072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9682 (2026-11-03)
 * Helpers:   ipow_i32, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A353072: Numbers k such that nextprime(k)-k is a positive square.
   The function incrementally generates terms and caches them.
   It uses next_prime(k) from utils and floor_sqrt for square testing. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/ipow_i32.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* offset starts at 1 */

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_k = 1;       /* next integer to test */

    /* Grow cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        uint64_t k = (uint64_t)next_k++;
        uint64_t p = next_prime(k);          /* smallest prime > k */
        uint64_t diff = p - k;               /* difference (positive) */

        /* diff is a square? */
        seq_t d = (seq_t)diff;
        seq_t s = floor_sqrt(d);
        if (s * s == d) {                    /* square check */
            /* ensure capacity */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) {
                    /* allocation failed: abort with 0 */
                    return (seq_t)0;
                }
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
