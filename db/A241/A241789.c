/*
 * A241789  Triangular numbers which have one or more occurrences of exactly
 *          six different digits.
 * https://oeis.org/A241789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2500, u64 n <= 2500, u128 n <= 2500,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 2045 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Check whether a triangular number has exactly six distinct decimal digits. */
static bool tri_has_property(seq_t tri)
{
    uint16_t mask = 0;                 /* 10 bits for digits 0–9 */
    seq_t t = tri;
    if (t == 0) return false;          /* T_0 = 0 has no digits */
    while (t > 0) {
        uint16_t d = (uint16_t)(t % 10);
        mask |= (1U << d);
        t /= 10;
    }
    /* Count set bits in mask. */
    int bits = 0;
    while (mask) {
        bits++;
        mask &= mask - 1;
    }
    return bits == 6;
}

/* The sequence: a(n) = the n‑th triangular number that uses
   exactly six distinct decimal digits.  Triangular numbers T_k
   are defined by T_k = k*(k+1)/2.  The algorithm simply tests
   each triangular number for the digit‑set condition and
   collects the qualifying values. */

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for offset 1, return 0 gracefully */
        return 0;

    /* Static cache for memoisation. */
    static idx_t last_checked = 0;          /* largest triangular index processed */
    static idx_t terms_found = 0;           /* number of qualifying terms stored */
    static seq_t *cache = NULL;             /* array of found terms */
    static size_t capacity = 0;             /* current allocation size */

    /* Return cached value if already computed. */
    if (n <= terms_found)
        return cache[n - 1];

    /* Allocate cache on first use. */
    if (!cache) {
        capacity = 64;
        cache = mem_new(seq_t, capacity);
    }

    /* Generate triangular numbers one by one until we have the nth term. */
    for (last_checked++; ; ++last_checked) {
        seq_t tri = (seq_t)last_checked * ((seq_t)last_checked + 1) / 2;
        if (tri_has_property(tri)) {
            /* Store if we found a new qualifying term. */
            if (terms_found == capacity) {
                capacity *= 2;
                cache = mem_resize(seq_t, cache, capacity);
            }
            cache[terms_found++] = tri;
            if (terms_found >= n)
                return cache[n - 1];
        }
    }
}
