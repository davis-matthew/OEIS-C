/*
 * A323831  a(0) = 5; thereafter a(n) is obtained by doubling a(n-1) and
 *          repeatedly deleting any string of identical digits.
 * https://oeis.org/A323831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 5; a(n) = repeatedly delete any run of identical
 *            consecutive digits from 2*a(n-1). (See OEIS A323831.)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 945 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: delete all maximal runs of identical consecutive digits
   (runs of length >= 2) from the decimal representation of x.
   The resulting number may still contain further runs; this
   function performs a single pass. */
static seq_t delete_runs_once(seq_t x)
{
    if (x == 0) return 0;           /* 0 has no digits to delete */

    /* Count digits. */
    idx_t dcnt = 0;
    seq_t tmp = x;
    while (tmp != 0) {
        tmp /= (seq_t)10;
        dcnt++;
    }

    /* Extract digits into an array (most significant first). */
    uint8_t *digits = (uint8_t*)malloc((size_t)dcnt);
    if (!digits) exit(1);          /* failure */
    tmp = x;
    size_t idx = (size_t)dcnt;
    while (tmp != 0) {
        digits[--idx] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }

    /* Produce the number after one pass of deletion. */
    seq_t result = 0;
    size_t i = 0;
    while (i < (size_t)dcnt) {
        size_t j = i + 1;
        while (j < (size_t)dcnt && digits[j] == digits[i]) j++;
        size_t len = j - i;
        if (len == 1) {
            result = result * (seq_t)10 + (seq_t)digits[i];
        }
        i = j;
    }

    free(digits);
    return result;
}

/* Helper: repeatedly apply delete_runs_once until fixed point. */
static seq_t delete_runs_cascade(seq_t x)
{
    seq_t prev = x;
    for (;;) {
        seq_t next = delete_runs_once(prev);
        if (next == prev) break;
        prev = next;
    }
    return prev;
}

/* Sequence accessor.  LRU-esque cache; works for any order of calls. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* If we already have the value, return it immediately. */
    if (cache_len > n) return cache[n];

    /* Initialize cache if empty. */
    if (cache_len == 0) {
        cache_cap = (n + 1 > 64) ? (n + 1) : 64;
        cache = mem_new(seq_t, (size_t)cache_cap);
        if (!cache) exit(1);
        cache[0] = (seq_t)5;
        cache_len = 1;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t doubled = prev * (seq_t)2;
        seq_t val = delete_runs_cascade(doubled);

        if (i >= cache_cap) {
            cache_cap = cache_cap * 2;
            cache = mem_resize(seq_t, cache, (size_t)cache_cap);
            if (!cache) exit(1);
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
