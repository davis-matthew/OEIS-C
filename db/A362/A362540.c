/*
 * A362540  Number of chordless cycles of length >= 4 in the n-flower graph.
 * https://oeis.org/A362540
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) -5*a(n-2) +4*a(n-3) -2*a(n-4) -4*a(n-5)
 *            +7*a(n-6) -8*a(n-7) +6*a(n-8) +4*a(n-9) -6*a(n-10) + a(n-12)
 *            Initial values: a(2)=3, a(3)=23, a(4)=63, a(5)=127, a(6)=273,
 *            a(7)=583, a(8)=1287, a(9)=2975, a(10)=6993, a(11)=16535,
 *            a(12)=39525, a(13)=95071
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"


/* Memoisation cache – a(2) stored at index 0 */
static seq_t *CACHE = NULL;
static size_t CACHE_SIZE = 0;  /* number of values stored */
static size_t CACHE_CAP = 0;   /* allocated capacity */

/* Base values for n = 2 .. 13 (12 entries) */
static const seq_t BASE[12] = {
    (seq_t)3,     /* a(2) */
    (seq_t)23,    /* a(3) */
    (seq_t)63,    /* a(4) */
    (seq_t)127,   /* a(5) */
    (seq_t)273,   /* a(6) */
    (seq_t)583,   /* a(7) */
    (seq_t)1287,  /* a(8) */
    (seq_t)2975,  /* a(9) */
    (seq_t)6993,  /* a(10) */
    (seq_t)16535, /* a(11) */
    (seq_t)39525, /* a(12) */
    (seq_t)95071  /* a(13) */
};

/* Ensure the cache can hold at least NEW_CAP entries */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= CACHE_CAP)
        return;
    size_t grow = CACHE_CAP ? (CACHE_CAP << 1) : 16;
    if (grow < new_cap)
        grow = new_cap;
    CACHE = seq_realloc(CACHE, CACHE_CAP, grow);
    CACHE_CAP = grow;
}

/* a(n) computes the number of chordless cycles of length >= 4
   in the n‑flower graph. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for index below offset */

    /* index into cache: a(2) → 0, a(n) → n‑2 */
    size_t idx = (size_t)(n - 2);

    /* initialise cache with known base values */
    if (CACHE_SIZE == 0) {
        size_t init = 12;                /* 12 base entries */
        ensure_cache_capacity(init);
        for (size_t i = 0; i < init; ++i)
            CACHE[i] = BASE[i];
        CACHE_SIZE = init;
    }

    /* grow cache if necessary for the requested index */
    if (idx + 1 > CACHE_CAP)
        ensure_cache_capacity(idx + 1);

    /* compute missing terms up to idx (iterative, linear scan) */
    for (size_t i = CACHE_SIZE; i <= idx; ++i) {
        if (i < 12)                    /* already initialised */
            continue;

        seq_t term = CACHE[i - 1] * (seq_t)4
                   - CACHE[i - 2] * (seq_t)5
                   + CACHE[i - 3] * (seq_t)4
                   - CACHE[i - 4] * (seq_t)2
                   - CACHE[i - 5] * (seq_t)4
                   + CACHE[i - 6] * (seq_t)7
                   - CACHE[i - 7] * (seq_t)8
                   + CACHE[i - 8] * (seq_t)6
                   + CACHE[i - 9] * (seq_t)4
                   - CACHE[i -10] * (seq_t)6
                   + CACHE[i -12];
        CACHE[i] = term;
    }
    CACHE_SIZE = idx + 1;   /* we now hold all values up to idx */

    return CACHE[idx];
}
