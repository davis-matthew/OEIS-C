/*
 * A345911  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has reverse-alternating sum 1.
 * https://oeis.org/A345911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n-th integer k such that the reverse‑alternating sum of the
   k‑th composition in standard order equals 1.
   The function searches sequentially through integers, caches all
   successful k values, and returns the (n−1)-th cached element. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Test whether a given k satisfies the condition. */
static bool good_k(idx_t k)
{
    idx_t temp = k;
    int bit_index = 0;       /* bit index from LSB (0‑based) */
    int sum = 0;             /* current alternating sum of differences */
    int sign = 1;            /* +1 for first difference, then alternates */
    int prev_pos = 0;        /* previous position of a 1‑bit (1‑based) */

    while (temp) {
        if (temp & 1ULL) {
            int pos = bit_index + 1;           /* 1‑based position */
            int diff = pos - prev_pos;         /* difference to previous */
            sum += sign * diff;
            sign = -sign;
            prev_pos = pos;
        }
        temp >>= 1;
        ++bit_index;
    }
    return sum == 1;
}

/* Cache of the found k values. */
static uint64_t *cache_uint = NULL;
static size_t cache_cap   = 0;
static size_t cache_len   = 0;
static uint64_t next_k    = 1;   /* next integer to test */

/* Ensure the cache can hold at least 'needed' elements. */
static void expand_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *new_ptr = (uint64_t *)realloc(cache_uint, new_cap * sizeof(uint64_t));
    if (!new_ptr) abort();
    cache_uint = new_ptr;
    cache_cap  = new_cap;
}

/* Required function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* guard against invalid 0 index */

    while (cache_len < (size_t)n) {
        if (good_k(next_k)) {
            expand_cache(cache_len + 1);
            cache_uint[cache_len++] = next_k;
        }
        ++next_k;
    }
    return (seq_t)cache_uint[n - 1];  /* cast to seq_t (may construct big int) */
}
