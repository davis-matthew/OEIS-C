/*
 * A345912  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has reverse-alternating sum -1.
 * https://oeis.org/A345912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n-th integer k such that the reverse-alternating sum of
 * the k-th composition in standard order equals -1.
 * This implementation searches sequentially over k, testing the
 * condition by analysing the set bits of k.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* External types supplied by the build system */
typedef uint64_t idx_t;      /* natural indexing type */

/* Test whether a given k satisfies the reverse‑alternating sum = -1 condition */
static bool satisfies(idx_t k)
{
    idx_t tmp = k;
    idx_t pos     = 1;   /* position of the current bit (starting at 1) */
    idx_t prev    = 0;   /* previous set‑bit position, 0 at the start */
    long long sum = 0;   /* alternating sum */
    int sign = 1;        /* +1 for the first difference, then -1, +1, ... */

    while (tmp) {
        if (tmp & 1U) {
            long long diff = (long long)(pos - prev);
            sum += sign * diff;
            sign = -sign;
            prev = pos;
        }
        tmp >>= 1;
        pos++;
    }

    return sum == -1;
}

/* Cache for already found terms */
static idx_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t next_k      = 1;          /* next k to test */

/* Ensure the cache has room for one more element */
static void ensure_cache_capacity(void)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        idx_t *new_ptr = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
        if (!new_ptr) abort();          /* out of memory */
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safety for n < 1 */

    while (cache_size < (size_t)n) {
        if (satisfies(next_k)) {
            ensure_cache_capacity();
            cache[cache_size++] = next_k;
        }
        next_k++;
    }
    return (seq_t)cache[n - 1];
}
