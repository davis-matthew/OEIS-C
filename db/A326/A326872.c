/*
 * A326872  BII-numbers of connectedness systems.
 * https://oeis.org/A326872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A326872 (BII‑numbers of connectedness systems) by an incremental
 * search over non‑negative integers.  The n‑th term is the n‑th integer for
 * which the set of bit positions of 1‑bits is closed under taking the
 * bitwise OR of any two overlapping indices.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether an integer m represents a valid BII‑number.  The integer
 * is interpreted as a set of indices S = { position of each 1‑bit in m }.
 * The property is: for every i, j ∈ S with (i & j) ≠ 0, the number
 * (i | j) must also belong to S.  */
static bool is_valid(seq_t m)
{
    if (m == (seq_t)0)               /* m = 0 (empty system) is valid */
        return true;

    /* Collect the indices of set bits in m (positions start at 1). */
    idx_t indices[128];
    size_t sz = 0;

    seq_t tmp = m;
    idx_t pos = 1;
    while (tmp != (seq_t)0) {
        if (tmp & (seq_t)1)
            indices[sz++] = pos;
        tmp >>= 1;
        ++pos;
        if (sz >= 128)               /* sanity guard */
            return false;
    }

    /* For every unordered pair (i, j) check the closure property. */
    for (size_t a = 0; a < sz; ++a) {
        seq_t i = (seq_t)indices[a];
        for (size_t b = a + 1; b < sz; ++b) {
            seq_t j = (seq_t)indices[b];
            if ((i & j) != (seq_t)0) {
                seq_t or_val = i | j;      /* must be in S */
                bool found = false;
                for (size_t c = 0; c < sz; ++c)
                    if (indices[c] == (idx_t)or_val) {
                        found = true;
                        break;
                    }
                if (!found)
                    return false;
            }
        }
    }
    return true;
}

/* The primary function requested by the OEIS wrapper.  It memoises results
 * to allow arbitrary order of calls. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, but harmless */

    /* Static cache data for incremental computation. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t count = 0;            /* how many values already stored */
    static seq_t next_candidate = (seq_t)0;  /* next m to test */

    /* Ensure we have enough values. */
    while (count < n) {
        /* Expand cache if necessary. */
        if (count == cap) {
            size_t new_cap = (cap == 0) ? 8 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(1);      /* memory error */
            cache = tmp;
            cap = new_cap;
        }

        /* Test current candidate. */
        if (is_valid(next_candidate))
            cache[count++] = next_candidate;

        ++next_candidate;
    }
    return cache[n - 1];
}
