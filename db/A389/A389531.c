/*
 * A389531  Standard composition numbers of compositions whose maximal runs R
 *          all satisfy length(R) != first(R).
 * https://oeis.org/A389531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   composition_good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Determination of A389531 – “Standard composition numbers of compositions whose
 * maximal runs R all satisfy length(R) != first(R)”.
 * Computation: iterate non‑negative integers k, map each k to a composition
 * via the standard binary ordering described in the OEIS entry, test the
 * property, and collect the qualifying k’s.  a(n) is the n‑th qualifying k
 * (1‑based). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/composition_good.h"

/* The implementation below is compatible with both the C and C++ builds.
 * It does not rely on realloc, allowing `seq_t` to be an arbitrary‑precision
 * integer class.  A static array of 100 000 elements suffices for all
 * required indices (n <= 10000). */
#define MAX_CACHE_SIZE 100000

static seq_t cache[MAX_CACHE_SIZE];
static size_t cache_size = 0;

/* Append a new term to the cache.  It is assumed that `cache_size`
 * is < MAX_CACHE_SIZE. */
static void cache_add(idx_t k)
{
    cache[cache_size++] = (seq_t)k;
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    static idx_t next_k = 0;    /* next k to test for inclusion */

    while (cache_size < n) {
        while (!composition_good(next_k)) {
            next_k++;
        }
        cache_add(next_k);
        next_k++;
    }
    return cache[n - 1];
}
