/*
 * A333299  Number of canonical sequences of moves of length n for the Rubik's
 *          cube puzzle using the quarter-turn metric.
 * https://oeis.org/A333299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1)+12*a(n-2)+8*a(n-3)+2*a(n-4) for n>4;
   initial values: a(0)=1, a(1)=12, a(2)=114, a(3)=1068, a(4)=10011 */
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static size_t computed = 0;      /* number of elements stored (== cache.size()) */

    if (cache.empty())
    {
        cache.resize(5);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)114;
        cache[3] = (seq_t)1068;
        cache[4] = (seq_t)10011;
        computed = 5;
    }

    if ((size_t)n < cache.size())
        return cache[(size_t)n];

    size_t needed = (size_t)n + 1;
    if (cache.size() < needed)
        cache.resize(needed);

    for (size_t i = computed; i < needed; ++i)
    {
        cache[i] = (seq_t)8 * cache[i-1] + (seq_t)12 * cache[i-2] +
                   (seq_t)8 * cache[i-3] + (seq_t)2 * cache[i-4];
    }
    computed = needed;
    return cache[(size_t)n];
#else
    static seq_t *cache = NULL;
    static size_t size = 0;          /* allocated size */
    static size_t computed = 0;      /* number of elements stored */

    if (cache == NULL)
    {
        size = 5;
        cache = (seq_t*)malloc(size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)114;
        cache[3] = (seq_t)1068;
        cache[4] = (seq_t)10011;
        computed = 5;
    }

    if (n < (idx_t)computed)
        return cache[(size_t)n];

    size_t needed = (size_t)n + 1;
    if (size < needed)
    {
        size_t newsize = (size > 0 ? size * 2 : 8);
        while (newsize < needed)
            newsize <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        cache = tmp;
        size = newsize;
    }

    for (size_t i = computed; i < needed; ++i)
    {
        cache[i] = (seq_t)8 * cache[i-1] + (seq_t)12 * cache[i-2] +
                   (seq_t)8 * cache[i-3] + (seq_t)2 * cache[i-4];
    }
    computed = needed;
    return cache[(size_t)n];
#endif
}
