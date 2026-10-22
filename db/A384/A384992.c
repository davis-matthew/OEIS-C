/*
 * A384992  Positive numbers k whose prime factors together with their
 *          exponents are distinct.
 * https://oeis.org/A384992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_valid_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A384992: numbers whose prime factors and exponents are all distinct */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_valid_candidate.h"

/* ----------  cached sequence storage  ---------- */
static seq_t *cache = NULL;          /* dynamic array of computed terms   */
static size_t cache_len = 0;         /* number of terms stored           */
static size_t cache_cap = 0;         /* allocated size of cache          */
static uint64_t last_candidate = 0;  /* last integer tested               */

static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t newcap = (cache_cap == 0) ? 1 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = newcap;
    }
    cache[cache_len++] = val;
}

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for index 0 – harmless guard */
        return 0;

    /* compute terms until the required index is reached */
    while (cache_len < (size_t)n) {
        uint64_t candidate = ++last_candidate;
        if (is_valid_candidate(candidate))
            cache_append((seq_t)candidate);
    }

    return cache[(size_t)n - 1];
}
