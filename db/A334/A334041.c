/*
 * A334041  (a(n-2) XOR a(n-1)) OR (highest bit of a(n-2))*2 OR 1; a(0)=2,
 *          a(1)=3.
 * https://oeis.org/A334041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=2, a(1)=3 a(n) = (a(n-2) XOR a(n-1)) OR (1 <<
 *            bitlen(a(n-2))) OR 1
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   bitlen_seq_t, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated capacity */
    static size_t  sz    = 0;   /* number of computed elements */

    /* initialise cache with the two base values */
    if (cap == 0) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        sz = 2;
    }

    if (n < 2)
        return cache[(size_t)n];

    /* grow capacity if needed */
    if (cap <= (size_t)n) {
        size_t newcap = cap > 0 ? cap << 1 : 2;
        while (newcap <= (size_t)n) newcap <<= 1;
        grow_seq_array(&cache, &cap, newcap);
    }

    /* compute all missing terms up to n */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        seq_t prev2 = cache[i-2];
        seq_t prev1 = cache[i-1];
        seq_t xored = prev2 ^ prev1;
        seq_t high  = ((seq_t)1) << bitlen_seq_t(prev2);
        cache[i] = xored | high | ((seq_t)1);
    }
    sz = (size_t)n + 1;
    return cache[(size_t)n];
}
