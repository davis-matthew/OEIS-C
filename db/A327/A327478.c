/*
 * A327478  Numbers whose average binary index is also a binary index.
 * https://oeis.org/A327478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   _append_value, _good_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n‑th positive integer whose average binary
 * index is also a binary index.  The binary index of m is the position
 * (starting at 1 for the least significant bit) of a 1 in the reversed
 * binary expansion of m.  The average index must be integer and that
 * integer position must also contain a 1.  This property is tested
 * directly in _good_index below. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/_append_value.h"
#include "../utils/_good_index.h"

/* External function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; unused entry for n=0 */
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next_candidate = 1;    /* next integer to test */

    while (cache_size < n) {
        if (_good_index(next_candidate))
            _append_value((seq_t)next_candidate, &cache, &cache_size, &cache_cap);
        next_candidate++;
    }
    return cache[n - 1];
}
