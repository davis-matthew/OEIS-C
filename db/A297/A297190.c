/*
 * A297190  Numbers not in A072099.
 * https://oeis.org/A297190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * A072099(n+1), where A072099(1) = 1 and A072099(k)
 *            is the smallest integer greater than A072099(k-1) that is not
 *            equal to j*A072099(j) for any j < k (computed by a sieve-like
 *            scan)
 * Range:     u8 n <= 14, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t m = n + 1;
    seq_t *prod = mem_new(seq_t, (size_t)m + 1);
    seq_t cand = 0;
    idx_t ptr = 0;
    for (idx_t k = 1; k <= m; ++k) {
        ++cand;
        for (;;) {
            while (ptr < k - 1 && prod[ptr] < cand) ++ptr;
            if (ptr < k - 1 && prod[ptr] == cand) ++cand;
            else break;
        }
        prod[k - 1] = (seq_t)k * cand;
    }
    seq_t result = prod[m - 1];
    mem_free(prod);
    return result;
}
