/*
 * A335039  Numbers whose binary representation encodes a (binary) max-heap on
 *          elements from the set {0,1} with root at the least significant
 *          bit.
 * https://oeis.org/A335039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 937, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer whose binary representation encodes a binary max‑heap on {0,1} with root at the LSB. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Check whether x satisfies the heap property. */
static bool is_valid(seq_t x)
{
    if (x == 0) return true;          /* 0 is a valid heap (empty). */
    if ((x & 1) == 0) return false;   /* Root must be 1 for all other numbers. */

    unsigned bits = bitlen_seq_t(x);
    for (unsigned i = 1; i < bits; ++i)
    {
        if ((x >> i) & 1)
        {
            unsigned parent = (i - 1) >> 1;
            if (((x >> parent) & 1) == 0) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* Not used for n >= 1. */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t next_candidate = 0;  /* Next integer to test. */

    while (cache_size < n)
    {
        /* Find the next valid number. */
        while (!is_valid((seq_t)next_candidate))
            ++next_candidate;

        /* Store it in the cache. */
        seq_t *tmp = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));
        if (!tmp) abort();          /* Allocation failure. */
        cache = tmp;
        cache[cache_size++] = (seq_t)next_candidate;

        ++next_candidate;          /* Advance for subsequent searches. */
    }
    return cache[n - 1];
}
