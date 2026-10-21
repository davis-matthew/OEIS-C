/*
 * A255570  Numbers whose binary representation traces a closed circuit in
 *          honeycomb lattice when its bits, from the least to the second most
 *          significant bit, are interpreted as directions to proceed at each
 *          vertex. (The most significant 1-bit is ignored).
 * https://oeis.org/A255570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer whose binary representation
 *            (discarding the most significant 1‑bit) encodes a closed walk on
 *            the honeycomb lattice: interpreting 0 as a left turn and 1 as a
 *            right turn at each vertex. The code synthesizes the sequence by
 *            brute‑force search
 * Range:     u8 n <= 4, u16 n <= 400, u32 n <= 9867, u64 n <= 9867,
 *            u128 n <= 9867, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Direction vectors for a 6‑fold symmetric honeycomb lattice. */
static const int da[6] = { 1,  1,  0, -1, -1,  0 };
static const int db[6] = { 0, -1, -1,  0,  1,  1 };

/* Test whether n yields a closed walk under the above rules. */
static bool is_closed(uint64_t n)
{
    /* n = 1 corresponds to the empty walk. */
    if (n <= 1) return true;

    /* Number of bits below the most significant 1-bit. */
    int pos = 63 - __builtin_clzll(n);   /* floor_log2(n) */

    int dir = 0;     /* heading before the first step (arbitrary). */
    int a   = 0;
    int b   = 0;

    for (int k = 0; k < pos; ++k) {
        int bit = (n >> k) & 1ULL;
        /* Turn: 0 = left (+1), 1 = right (-1). */
        dir = (bit == 0) ? (dir + 1) % 6 : (dir + 5) % 6;
        a += da[dir];
        b += db[dir];
    }
    return (a == 0 && b == 0);
}

/* ----------------- Cached sequence generation ----------------- */

static seq_t *cache = NULL;      /* dynamic array of known terms */
static size_t cache_len = 0;      /* number of stored terms */
static size_t cache_cap = 0;      /* current capacity */
static uint64_t next_candidate = 1;  /* next integer to test */

/* Ensure the cache can hold at least 'minlen' elements. */
static void ensure_cache_capacity(size_t minlen)
{
    if (minlen <= cache_cap) return;
    size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
    while (new_cap < minlen) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Generate terms until the cache contains at least (idx+1) elements. */
static void grow_cache_to(size_t idx)
{
    while (cache_len <= idx) {
        uint64_t cand = next_candidate++;
        if (is_closed(cand)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        }
    }
}

/* ----------------- Public function ----------------- */

seq_t a(idx_t n)
{
    grow_cache_to(n);
    return cache[n];
}
