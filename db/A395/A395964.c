/*
 * A395964  Second iterate of the Thue-Morse transform applied to the
 *          Fibonacci word A003849.
 * https://oeis.org/A395964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = second iterate of the Thue-Morse transform applied to the
 *            Fibonacci word (fixed point of 0->01, 1->0)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   thue_morse_transform
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/thue_morse_transform.h"

/* static cache for all computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Generate the first 'len' bits of the Fibonacci word, apply T twice and store in cache. */
static void ensure_cache(idx_t needed_len)
{
    if (needed_len <= cache_len)
        return;

    /* free old cache if any */
    free(cache);

    /* ---------- generate Fibonacci word prefix (at least needed_len) ---------- */
    uint8_t *cur = (uint8_t*)malloc(1);
    cur[0] = 0;
    idx_t cur_len = 1;
    while (cur_len < needed_len) {
        /* count zeros in current word */
        idx_t zeros = 0;
        for (idx_t i = 0; i < cur_len; ++i)
            if (cur[i] == 0) ++zeros;

        idx_t new_len = cur_len + zeros;            /* each 0 expands to 2 chars, 1 to 1 */
        uint8_t *next = (uint8_t*)malloc(new_len);
        idx_t pos = 0;
        for (idx_t i = 0; i < cur_len; ++i) {
            if (cur[i] == 0) {
                next[pos++] = 0;
                next[pos++] = 1;
            } else {
                next[pos++] = 0;
            }
        }
        free(cur);
        cur = next;
        cur_len = new_len;
    }

    /* copy the prefix we need into a buffer of exact size */
    uint8_t *w0 = (uint8_t*)malloc(needed_len * sizeof(uint8_t));
    for (idx_t i = 0; i < needed_len; ++i)
        w0[i] = cur[i];
    free(cur);

    /* ---------- first Thue-Morse transform ---------- */
    uint8_t *w1 = (uint8_t*)malloc(needed_len * sizeof(uint8_t));
    thue_morse_transform(w0, w1, needed_len);

    /* ---------- second Thue-Morse transform ---------- */
    uint8_t *w2 = (uint8_t*)malloc(needed_len * sizeof(uint8_t));
    thue_morse_transform(w1, w2, needed_len);

    /* ---------- store results in cache ---------- */
    cache = (seq_t*)malloc(needed_len * sizeof(seq_t));
    for (idx_t i = 0; i < needed_len; ++i)
        cache[i] = (seq_t)w2[i];

    /* free temporary arrays */
    free(w0);
    free(w1);
    free(w2);

    cache_len = needed_len;
}

seq_t a(idx_t n)
{
    if (n >= cache_len)
        ensure_cache(n + 1);
    return cache[n];
}
