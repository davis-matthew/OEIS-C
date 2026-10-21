/*
 * A296699  Numbers whose base-5 digits d(m), d(m-1), ... d(0) have #(rises) <
 *          #(falls); see Comments.
 * https://oeis.org/A296699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_fewer_rises_than_falls
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for numbers whose base‑5 digits have fewer
   rises than falls.  For each n, the nth such number is returned.
   The algorithm simply examines successive integers until the
   required quantity of terms has been collected.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_fewer_rises_than_falls.h"

/* A simple cache for the computed terms.  The exact capacity is
   not critical; 200 000 comfortably covers n up to 10 000.  */
#define CACHE_CAP 200000
static seq_t cache[CACHE_CAP];
static size_t cache_size = 0;
static idx_t next_candidate = 1;   /* next integer to test */

/* Extend the cache until it contains at least n terms.  */
static void ensure_cached(idx_t n)
{
    while (cache_size < n && cache_size < CACHE_CAP) {
        /* Find the next integer belonging to the sequence. */
        while (!has_fewer_rises_than_falls((seq_t)next_candidate))
            ++next_candidate;

        cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* Main entry point.  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined but harmless */
    ensure_cached(n);
    return cache[n - 1];
}
