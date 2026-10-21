/*
 * A283161  Natural numbers whose digits can be formed by typing non-adjacent
 *          keys on a 123-456-789 keypad without repeating a digit.
 * https://oeis.org/A283161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 405, u32 n <= 453, u64 n <= 453,
 *            u128 n <= 453, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by generating all natural numbers that can be typed
 * with non‑adjacent keys on a 123‑456‑789 keypad without repeating a digit. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Adjacency bitmasks for digits 1‑9 (bit d corresponds to digit d). */
static const unsigned short adjmask[10] = {
    0,
    (1U << 2) | (1U << 4),                    /* 1 */
    (1U << 1) | (1U << 3) | (1U << 5),        /* 2 */
    (1U << 2) | (1U << 6),                    /* 3 */
    (1U << 1) | (1U << 5) | (1U << 7),        /* 4 */
    (1U << 2) | (1U << 4) | (1U << 6) | (1U << 8), /* 5 */
    (1U << 3) | (1U << 5) | (1U << 9),        /* 6 */
    (1U << 4) | (1U << 8),                    /* 7 */
    (1U << 5) | (1U << 7) | (1U << 9),        /* 8 */
    (1U << 6) | (1U << 8)                     /* 9 */
};

/* Global storage for the sequence. */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static bool   seq_built = false;

/* Append a value to the cached sequence, growing the buffer if needed. */
static void push_val(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 1024;
        seq_cache = seq_realloc(seq_cache, seq_cap, new_cap);
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* Recursively collect all valid numbers of a fixed length. */
static void dfs_collect(int pos, seq_t cur,
                        unsigned used, unsigned adj_used, int len)
{
    if (pos == len) {
        push_val(cur);
        return;
    }
    for (int d = 1; d <= 9; ++d) {
        unsigned bit = 1U << d;
        if (used & bit)                continue; /* digit already used   */
        if (adj_used & bit)            continue; /* adjacent to a used digit */
        dfs_collect(pos + 1,
                    cur * (seq_t)10 + (seq_t)d,
                    used | bit,
                    adj_used | adjmask[d],
                    len);
    }
}

/* Build the entire sequence once. */
static void build_all(void)
{
    if (seq_built) return;
    seq_cap   = 1024;
    seq_cache = seq_alloc(seq_cap);
    seq_len   = 0;
    for (int len = 1; len <= 9; ++len) {
        dfs_collect(0, 0, 0, 0, len);
    }
    seq_built = true;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (!seq_built) build_all();
    if (n == 0) return 0;
    if ((size_t)(n-1) < seq_len) return seq_cache[(size_t)(n-1)];
    return 0;   /* n beyond the generated terms – none defined. */
}
