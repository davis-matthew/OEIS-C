/*
 * A332227  Odd numbers k such that sigma(k) is congruent to 2 modulo 8.
 * https://oeis.org/A332227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 2158, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 78 terms through n = 8469 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A332227: odd k such that sigma(k) ≡ 2 (mod 8) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* helper from the utils library */

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* External linkage function required by the test harness */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    /* static variable that keeps track of the next odd number to test */
    static idx_t next_candidate = 1;
    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
        cache_len = 0;
        next_candidate = 1;
    }
    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* test only odd numbers */
        seq_t sigma = sigma_of(next_candidate);
        if ((sigma & (seq_t)7) == (seq_t)2) {
            /* store the candidate */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        /* move to the next odd number */
        next_candidate += 2;
    }
    /* return the requested term (1‑based index) */
    return cache[n - 1];
}
