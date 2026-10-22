/*
 * A322854  Number of compositions (ordered partitions) of n into hexagonal
 *          pyramidal numbers (A002412).
 * https://oeis.org/A322854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 51, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   hex_pyramidal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: number of compositions of n into hexagonal pyramidal numbers. Computed by DP. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/hex_pyramidal.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

static seq_t *hex_vals = NULL;
static idx_t hex_len = 0;
static size_t hex_cap = 0;
static idx_t next_k = 1;

static void ensure_cache(idx_t m)
{
    if (m < cache_len)
        return;

    if ((size_t)(m + 1) > cache_cap) {
        size_t new_cap = (m + 1) * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    while (true) {
        seq_t hp = hex_pyramidal(next_k);
        if ((seq_t)m < hp)
            break;
        if (hex_len == hex_cap) {
            size_t new_hex_cap = hex_cap ? hex_cap * 2 : 8;
            hex_vals = (seq_t *)realloc(hex_vals, new_hex_cap * sizeof(seq_t));
            hex_cap = new_hex_cap;
        }
        hex_vals[hex_len++] = hp;
        ++next_k;
    }

    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    for (idx_t i = cache_len; i <= m; ++i) {
        seq_t sum = 0;
        for (idx_t j = 0; j < hex_len; ++j) {
            seq_t h = hex_vals[j];
            if (h > (seq_t)i)
                break;
            sum += cache[i - (idx_t)h];
        }
        cache[i] = sum;
    }
    cache_len = m + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
