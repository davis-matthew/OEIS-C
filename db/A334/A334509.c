/*
 * A334509  Eventual period of a single cell in rule 41 cellular automaton in
 *          a cyclic universe of width n.
 * https://oeis.org/A334509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n > 14, a(n) = 2*a(n-4) – a(n-8). Initial values: a(1)=2,
 *            a(2)=2, a(3)=2, a(4)=2, a(5)=15, a(6)=2, a(7)=28, a(8)=8,
 *            a(9)=36, a(10)=20, a(11)=44, a(12)=12, a(13)=52, a(14)=28
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq.h"
#include <string.h>

/* Cache to store computed values */
static seq_t *cache = NULL;
static idx_t cached_max = 0;

/* Initialise cache with known initial values */
static void init_cache(void)
{
    if (cache == NULL) {
        /* Allocate space for indices 0..14 (index 0 unused) */
        size_t init_size = 15;
        cache = alloc_seq(init_size);
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)15;
        cache[6] = (seq_t)2;
        cache[7] = (seq_t)28;
        cache[8] = (seq_t)8;
        cache[9] = (seq_t)36;
        cache[10] = (seq_t)20;
        cache[11] = (seq_t)44;
        cache[12] = (seq_t)12;
        cache[13] = (seq_t)52;
        cache[14] = (seq_t)28;
        cached_max = 14;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* values for n<1 are irrelevant */

    /* Initialise cache if not already done */
    init_cache();

    /* If requested index is already computed, return it */
    if (n <= cached_max)
        return cache[n];

    /* Extend cache to accommodate index n */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = alloc_seq(new_size);
    memcpy(new_cache, cache, (size_t)(cached_max + 1) * sizeof(seq_t));

    /* Compute values up to n using the recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i <= 14) {
            /* Initial values are already set */
            continue;
        }
        /* a(i) = 2*a(i-4) - a(i-8) */
        seq_t val = cache[i - 4];
        val = val * ((seq_t)2) - cache[i - 8];
        new_cache[i] = val;
    }

    /* Free old cache and update static state */
    free(cache);
    cache = new_cache;
    cached_max = n;

    return cache[n];
}
