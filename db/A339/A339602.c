/*
 * A339602  a(n) = (a(n-2) XOR A030101(a(n-1))) + 1, a(0) = 0, a(1) = 1.
 * https://oeis.org/A339602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = (a(n-2) XOR A030101(a(n-1))) + 1 a(0) = 0, a(1) = 1
 * Range:     u8 n <= 281, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

/* Bit‑reversal of a seq_t value. */
static seq_t bitrev(seq_t x)
{
    unsigned len = bitlen_seq_t(x);   /* minimal number of bits to represent x */
    seq_t rev = 0;
    for (unsigned i = 0; i < len; ++i)
    {
        rev = (rev << 1) | ((x >> i) & 1);
    }
    return rev;
}

/* Memoised computation of A339602. */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* highest index + 1 */
    static size_t cache_cap  = 0;   /* allocated size */

    /* initialise cache for the first non‑trivial call */
    if (cache_size == 0)
    {
        cache_cap   = 2;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]    = (seq_t)0;
        cache[1]    = (seq_t)1;
        cache_size  = 2;
    }

    /* if the requested index is beyond the cached range, extend the cache */
    if ((size_t)n >= cache_size)
    {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;

        for (idx_t i = (idx_t)cache_size; i <= n; ++i)
        {
            seq_t prev2 = cache[i - 2];
            seq_t prev1 = cache[i - 1];
            cache[i] = (prev2 ^ bitrev(prev1)) + (seq_t)1;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
