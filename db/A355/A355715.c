/*
 * A355715  a(0) = 0; for n > 0, a(n) is the total number of binary bits that
 *          n has in common with all previous terms.
 * https://oeis.org/A355715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 9568, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A355715: a(n) counts the total number of equal bits between n
 * and all previous terms a(k), k < n. For each bit position i
 * (0‑based) that satisfies max(bitlen(n),bitlen(a(k))) > i,
 * if bit_i(n)==bit_i(a(k)) then it contributes 1 to the sum for
 * this k.  a(0)=0.  This implementation keeps per‑bit counts of
 * zeros and ones among all computed a(k) and uses them to obtain
 * a(n) in O(1) per bit, resulting in overall O(n·B) time (B = max
 * bitlen seen).  */

/* Include the utility that gives the bit length of a seq_t value. */
#include <stdlib.h>
#include <string.h>
#include "../utils/bitlen_seq_t.h"

/* Per‑bit zero/one counters and their dynamic capacity. */
static idx_t *count0 = NULL;
static idx_t *count1 = NULL;
static idx_t  count_cap = 0;

/* Ensure the zero/one counter arrays have room for at least 'bits' entries. */
static void ensure_count_capacity(idx_t bits)
{
    if (count_cap == 0) {
        count_cap = 64;                           /* initial capacity */
        count0 = (idx_t *)calloc(count_cap, sizeof(idx_t));
        count1 = (idx_t *)calloc(count_cap, sizeof(idx_t));
    } else while (bits > count_cap) {
        idx_t new_cap = count_cap * 2;
        count0 = (idx_t *)realloc(count0, new_cap * sizeof(idx_t));
        count1 = (idx_t *)realloc(count1, new_cap * sizeof(idx_t));
        memset(count0 + count_cap, 0, (new_cap - count_cap) * sizeof(idx_t));
        memset(count1 + count_cap, 0, (new_cap - count_cap) * sizeof(idx_t));
        count_cap = new_cap;
    }
}

/* Update the per‑bit counters with all bits of the given value. */
static void add_to_counts(seq_t v)
{
    idx_t bl = bitlen_seq_t(v);                     /* bit length of v */
    if (bl == 0) bl = 1;                            /* treat 0 as one bit */
    ensure_count_capacity(bl);

    for (idx_t i = 0; i < bl; ++i) {
        seq_t bit = (v >> i) & (seq_t)1;
        if (bit)
            count1[i]++;                            /* bit i is 1 */
        else
            count0[i]++;                            /* bit i is 0 */
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache     = NULL;
    static idx_t  cache_size = 0;
    static idx_t  cache_cap  = 0;

    /* First call: initialise cache and count arrays. */
    if (cache == NULL) {
        cache_cap   = 16;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]    = (seq_t)0;                      /* a(0) = 0 */
        cache_size  = 1;
        add_to_counts(cache[0]);                    /* include a(0) in counts */
    }

    /* Compute terms up to n (inclusive). */
    while (cache_size <= n) {
        idx_t val = cache_size;                     /* the integer whose a() we need */
        idx_t bl  = bitlen_seq_t((seq_t)val);
        if (bl == 0) bl = 1;                        /* treat 0 as one bit */

        /* Ensure counts arrays are large enough for this bit length. */
        ensure_count_capacity(bl);

        /* Sum equal bits with all previous terms. */
        idx_t sum = 0;
        for (idx_t i = 0; i < bl; ++i) {
            seq_t bit = ((seq_t)val >> i) & (seq_t)1;
            if (bit)
                sum += count1[i];
            else
                sum += count0[i];
        }

        /* Grow cache if necessary. */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_size++] = (seq_t)sum;           /* store a(n) */
        add_to_counts((seq_t)sum);                 /* update counts for future */
    }

    return cache[(size_t)n];
}
