/*
 * A307270  Coordination sequence for trivalent node of type alpha''' in the
 *          first Moore pentagonal tiling.
 * https://oeis.org/A307270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A307270
    Recurrence:   a(n) = 2*a(n-4) - a(n-8)   for n > 17
    Initial values: a(0)=1, a(1)=3, a(2)=6, a(3)=9, a(4)=13, a(5)=18,
                    a(6)=27, a(7)=31, a(8)=34, a(9)=41, a(10)=50,
                    a(11)=58, a(12)=61, a(13)=64, a(14)=75, a(15)=81,
                    a(16)=82, a(17)=85, a(18)=100, a(19)=104,
                    a(20)=103, a(21)=106, a(22)=125, a(23)=127
    Note: this implementation memoises computed values to allow
    arbitrary call order.  It uses the above recurrence for all n
    >= 24.  The table for the first 24 terms is hard‑coded because
    the recurrence has different behaviour before n=24.
    The implementation is valid both under C11 and C++17.  The type
    seq_t is assumed to support the usual arithmetic operators.
*/

#include <stdlib.h>
#include <stddef.h>

static const seq_t init[24] = {
    1, 3, 6, 9, 13, 18, 27, 31,
    34, 41, 50, 58, 61, 64, 75, 81,
    82, 85, 100, 104, 103, 106, 125, 127
};

static seq_t *cache = NULL;   /* memoised values */
static size_t cache_len = 0;   /* number of elements stored (size of cache) */

/* Main function */
seq_t a(idx_t n)
{
    /* Convert n to a normal size_t for indexing; we assume n fits in size_t */
    size_t idx = (size_t)n;

    /* Grow cache if necessary */
    if (idx >= cache_len)
    {
        size_t new_len = idx + 1;

        /* Realloc cache */
        seq_t *new_cache = NULL;
        if (cache == NULL)
            new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        else
            new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        /* Handle allocation failure in a defensive way */
        if (new_cache == NULL)
            return (seq_t)0;      /* fallback – caller should not request too large n */

        cache = new_cache;

        /* Compute newly needed values */
        for (size_t i = cache_len; i < new_len; ++i)
        {
            if (i < 24)
                cache[i] = init[i];
            else
                /* a(i) = 2*a(i-4) - a(i-8)   (valid for i >= 24) */
                cache[i] = (cache[i - 4] * 2) - cache[i - 8];
        }

        cache_len = new_len;
    }

    return cache[idx];
}
