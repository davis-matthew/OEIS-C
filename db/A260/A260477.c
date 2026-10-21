/*
 * A260477  Numbers n where n^2 is an anagram of (n+1)^2 in base 2.
 * https://oeis.org/A260477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 7638, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A260477 – Numbers n where n² is a binary anagram of (n+1)²
 *
 *  The routine searches for the n‑th integer n satisfying:
 *      bitlen(n²)   == bitlen((n+1)²)
 *      popcount(n²) == popcount((n+1)²)
 *
 *  Results are cached so repeated calls are fast and the order of calls
 *  does not matter.
 */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/grow_seq.h"

/* Return the popcount (number of 1‑bits) of a seq_t value. */
static inline idx_t popcount_seq_t(seq_t x)
{
    idx_t cnt = 0;
    while (x) {
        if (x & 1) ++cnt;
        x >>= 1;
    }
    return cnt;
}

/* Test whether n satisfies the A260477 condition. */
static inline bool good_n(idx_t n)
{
    seq_t k     = (seq_t)n;
    seq_t sq    = k * k;
    seq_t sq1   = (k + (seq_t)1) * (k + (seq_t)1);

    if (bitlen_seq_t(sq) != bitlen_seq_t(sq1))
        return false;

    return popcount_seq_t(sq) == popcount_seq_t(sq1);
}

/* ------------------------------------------------------------------
 *  Cached evaluator of the sequence
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of found terms          */
    static size_t cache_cap = 0;    /* current capacity of cache     */
    static size_t cache_size = 0;   /* number of terms stored        */
    static idx_t next_candidate = 1;/* next n to test                */

    /* Extend the cache until it contains n elements. */
    while (cache_size < n) {
        if (good_n(next_candidate)) {
            /* Grow the cache if necessary. */
            if (cache_size >= cache_cap)
                grow_seq(&cache, &cache_cap,
                         cache_size + 1);     /* guarantee space */

            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];      /* 1‑based indexing per OEIS */
}
