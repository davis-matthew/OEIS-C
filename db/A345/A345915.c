/*
 * A345915  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has alternating sum <= 0.
 * https://oeis.org/A345915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function iterates over compositions in standard order (A066099)
   and returns the k‑th composition index whose alternating sum <= 0.
   The alternating sum condition is tested by generating each composition
   from the binary representation of the composition index. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Check whether the composition corresponding to index k has alternating
   sum <= 0. */
static bool qualifies(idx_t k)
{
    int64_t sum = 0;
    bool sign = true;               /* true for positive, false for negative */

    /* In a 64‑bit integer there are at most 64 set bits.  We use a small
       array on the stack to store the differences. */
    uint8_t diff[64];
    size_t r = 0;
    uint8_t lastPos = 0;            /* position of the previous 1 (in 1‑based) */

    idx_t word = k;
    uint8_t bit_index = 0;

    /* Scan the set bits in ascending order.  For each set bit (at
       position bit_index) we add 1 to make positions 1‑based. */
    while (word)
    {
        if (word & 1)
        {
            uint8_t current = bit_index + 1;
            diff[r++] = current - lastPos;
            lastPos = current;
        }
        word >>= 1;
        bit_index++;
    }

    /* Empty composition (k == 0) has alternating sum 0. */
    if (r == 0)
        return true;

    /* Compute the alternating sum of the reversed difference array. */
    for (size_t j = 0; j < r; ++j)
    {
        int64_t part = diff[r - 1 - j];
        sum += sign ? part : -part;
        sign = !sign;
    }
    return sum <= 0;
}

/* Return the n‑th term of sequence A345915 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset 1 guarantees this is never used */
        return (seq_t)0;

    /* Static cache of already found qualifying indices.  The cache is
       extended as needed. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_sz = 0;
    static idx_t cur_k = 0;   /* the next composition index to test */

    if (cache_sz < n)
    {
        if (!cache)
        {
            cache_cap = 256;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            cache_sz = 0;
            cur_k = 0;
        }

        while (cache_sz < n)
        {
            if (qualifies(cur_k))
            {
                if (cache_sz == cache_cap)
                {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_sz++] = (seq_t)cur_k;
            }
            cur_k++;
        }
    }

    return cache[n - 1];
}
