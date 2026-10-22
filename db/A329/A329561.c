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
 * Helpers:   is_BII_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_BII_uint64.h"

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
                void *tmp = realloc(cache, capacity * sizeof(seq_t));
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
