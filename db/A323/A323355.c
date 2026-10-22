/*
 * A323355  a(1)=1; for n >= 2, a(n) = Sum_{i=1..A000120(n)} a(z(i)), where
 *          z(i) are the positions of 1's in the binary expansion of n,
 *          counted from left to right.
 * https://oeis.org/A323355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = Σ_{i in positions of 1‑bits of n
 *            (left‑to‑right)} a(i)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is memoised in a static array.  It works for any unsigned       */
/* integer type seq_t and for arbitrary‑precision seq_t (operator overloading   */
/* provides the usual arithmetic ops).  The cache is grown on demand.         */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static memoisation cache; cache[0] is unused */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 4;                    /* will store indices 0..3 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        cache[1] = 1;                     /* a(1) = 1 */
    }

    /* expand cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;     /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* initialise new entries to zero */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* already computed?  a(n) is never zero for n ≥ 1, so zero means "not yet" */
    if (cache[n] != 0) return cache[n];

    /* determine the length L of the binary representation of n */
    idx_t temp = n;
    idx_t L = 0;
    while (temp != 0) {
        temp >>= 1;
        ++L;
    }

    /* sum a(position) for each set bit; position counted from left (MSB = 1) */
    seq_t sum = 0;
    temp = n;
    idx_t bit_index = 0;                 /* 0 = least‑significant bit */
    while (temp != 0) {
        if (temp & (idx_t)1) {
            idx_t pos = L - bit_index;   /* left‑to‑right position of the bit */
            sum += a(pos);               /* recursive memoised call */
        }
        temp >>= 1;
        ++bit_index;
    }

    cache[n] = sum;
    return sum;
}
