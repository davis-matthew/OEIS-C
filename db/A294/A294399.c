/*
 * A294399  Solution of the complementary equation a(n) = a(n-1) + b(n-2) + 3,
 *          where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A294399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + b(n‑2) + 3, a(0)=1, a(1)=3, * b(0)=2, b(1)=4;
 *            b(n) is the smallest unused positive integer
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   ensure_bitset_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/ensure_bitset_capacity.h"

static inline bool bit_is_set(seq_t val, const uint8_t *bits)
{
    size_t idx = (size_t)(unsigned long long)val;
    return (bits[idx >> 3] >> (idx & 7)) & 1u;
}

static inline void bit_set(seq_t val, uint8_t *bits)
{
    size_t idx = (size_t)(unsigned long long)val;
    bits[idx >> 3] |= (uint8_t)(1u << (idx & 7));
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *a_cache = NULL;
    static seq_t *b_cache = NULL;
    static size_t a_cap   = 0, b_cap   = 0;
    static idx_t  max_idx = (idx_t)-1;        /* uninitialised sentinel   */
    static uint8_t *used_bits = NULL;
    static size_t used_bytes = 0;
    static seq_t next_free = 5;               /* first unused integer */

    /* one‑time initialisation */
    if (max_idx == (idx_t)-1) {
        a_cap = 8;
        b_cap = 8;
        a_cache = (seq_t *)calloc(a_cap, sizeof(seq_t));
        b_cache = (seq_t *)calloc(b_cap, sizeof(seq_t));
        if (!a_cache || !b_cache) exit(EXIT_FAILURE);

        /* initialise first four terms */
        a_cache[0] = (seq_t)1;  a_cache[1] = (seq_t)3;
        b_cache[0] = (seq_t)2;  b_cache[1] = (seq_t)4;

        /* mark used numbers 1..4 */
        for (seq_t v = 1; v <= 4; ++v)
            ensure_bitset_capacity(v, &used_bits, &used_bytes);
        bit_set((seq_t)1, used_bits);
        bit_set((seq_t)2, used_bits);
        bit_set((seq_t)3, used_bits);
        bit_set((seq_t)4, used_bits);

        max_idx = 1;
    }

    if (n <= max_idx)
        return a_cache[n];

    /* enlarge caches if necessary */
    if ((size_t)(n + 1) > a_cap) {
        size_t new_cap = a_cap;
        while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_cache = tmp;
        a_cap = new_cap;
    }
    if ((size_t)(n + 1) > b_cap) {
        size_t new_cap = b_cap;
        while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        b_cache = tmp;
        b_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = max_idx + 1; i <= n; ++i) {
        /* compute a[i] */
        seq_t ai = a_cache[i - 1] + b_cache[i - 2] + (seq_t)3;
        a_cache[i] = ai;

        /* mark a[i] as used */
        ensure_bitset_capacity(ai, &used_bits, &used_bytes);
        bit_set(ai, used_bits);

        /* find smallest unused integer for b[i] */
        seq_t bi = next_free;
        for (;;) {
            ensure_bitset_capacity(bi, &used_bits, &used_bytes);
            if (!bit_is_set(bi, used_bits))
                break;
            ++bi;
        }
        b_cache[i] = bi;
        next_free = bi + 1;
        bit_set(bi, used_bits);
    }

    max_idx = n;
    return a_cache[n];
}
