/*
 * A326880  BII-numbers of set-systems that are closed under nonempty
 *          intersection.
 * https://oeis.org/A326880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 180, u16 n <= 9084, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bii_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A326880: BII-numbers of set-systems that are closed under nonempty intersection.
 *
 * For an integer m the binary indices of m are the positions of 1-bits in its
 * binary representation (least significant bit has index 1).  The set-system
 * associated with m consists of, for each binary index p of m, the binary
 * indices of p.  m is a BII-number iff this set-system is closed under
 * non‑empty intersection.
 *
 * The function enumerates m=0,1,2,… accumulating those that satisfy the
 * property until it has produced n terms.  The result is cached so that
 * subsequent calls, even out of order, run in O(1) for already computed
 * terms.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include "../utils/bii_valid.h"

/* The sequence function.  Results are cached up to n=10000. */
seq_t a(idx_t n)
{
    /* Capacity must comfortably exceed 10000. */
    #define CACHE_CAP 20000
    static seq_t cache[CACHE_CAP];
    static size_t cache_len = 0;
    static idx_t next = 0;

    /* Grow cache if needed (unlikely with the defined capacity). */
    while (cache_len < n) {
        if (bii_valid(next))
            cache[cache_len++] = (seq_t)next;
        next++;
    }
    return cache[n - 1];
}
