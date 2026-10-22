/*
 * A393468  Number of minimal dominating sets in the n-necklace graph.
 * https://oeis.org/A393468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=6, a(1)=4, a(2)=10, a(3)=61, a(4)=234, a(5)=919 a(n) =
 *            4*a(n-1) - 3*a(n-2) + 11*a(n-3) - 6*a(n-4) + 16*a(n-5) -
 *            8*a(n-6) for n>=6 This recurrence follows from the rational
 *            generating function of the sequence
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The types seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache     = NULL;
    static idx_t cache_size = 0;   /* number of terms stored (cache[0..cache_size-1]) */
    static idx_t cache_cap  = 0;   /* allocated size of cache */

    /* allocate initial cache */
    if (cache == NULL) {
        cache_cap = 64;                          /* initial capacity */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* enlarge cache if required */
    if (n >= cache_size) {
        /* ensure enough capacity */
        while (n >= cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* compute missing terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i == 0)  val = 6;
            else if (i == 1)  val = 4;
            else if (i == 2)  val = 10;
            else if (i == 3)  val = 61;
            else if (i == 4)  val = 234;
            else if (i == 5)  val = 919;
            else {
                /* recurrence: a(i) = 4*a(i-1) - 3*a(i-2) + 11*a(i-3)
                                   - 6*a(i-4) + 16*a(i-5) - 8*a(i-6) */
                val = 4 * cache[i-1];
                val -= 3 * cache[i-2];
                val += 11 * cache[i-3];
                val -= 6 * cache[i-4];
                val += 16 * cache[i-5];
                val -= 8 * cache[i-6];
            }
            cache[i] = val;
        }
        cache_size = n + 1;   /* we have computed up to n */
    }

    return cache[n];
}
