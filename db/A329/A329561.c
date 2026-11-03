/*
 * A329561  BII-numbers of intersecting antichains of sets.
 * https://oeis.org/A329561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th BII-number A BII-number is defined by the set of its
 *            binary indices and the binary indices of each binary index. The
 *            set-system is counted if it is intersecting (no two edges
 *            disjoint) and an antichain (no proper subset relation). a(n) is
 *            the n-th such number, with a(1)=0
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether a number n is a BII-number.  */
static inline bool is_BII_uint64(uint64_t n)
{
    if (n == 0) return true;          /* empty set-system */

    /* Each set bit j of n contributes an edge whose mask is (j+1). */
    uint64_t masks[64];
    int m = 0;
    uint64_t bits = n;
    while (bits) {
        int j = __builtin_ctzll(bits);    /* position of the lowest set bit */
        bits &= bits - 1;                 /* clear that bit */
        uint64_t mask = (uint64_t)(j + 1); /* p = j+1 */
        masks[m++] = mask;
    }

    /* Intersecting and antichain tests. */
    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m; ++j) {
            uint64_t a = masks[i];
            uint64_t b = masks[j];
            if ((a & b) == 0)            /* disjoint → not intersecting */
                return false;
            if (a != b && (a & b) == a)   /* a ⊂ b → not antichain */
                return false;
            if (a != b && (a & b) == b)   /* b ⊂ a → not antichain */
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Lazy cache of computed BII-numbers. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t cached = 0;
    static idx_t tested = 0;          /* next candidate to test */

    if (n < 1) return 0;              /* undefined below the offset */

    while (cached < n) {
        if (is_BII_uint64((uint64_t)tested)) {
            if (cached == capacity) {
                capacity = capacity ? capacity * 2 : 64;
                void *tmp = mem_resize(seq_t, cache, capacity);
                if (!tmp) {            /* allocation failure – abort */
                    return 0;
                }
                cache = (seq_t*)tmp;
            }
            cache[cached++] = (seq_t)tested;
        }
        ++tested;
    }

    return cache[n - 1];
}
