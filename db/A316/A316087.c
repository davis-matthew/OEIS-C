/*
 * A316087  Expansion of 1/(1 + Sum_{k>=1} k^2 * x^k).
 * https://oeis.org/A316087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{k=1..n} k² * a(n-k) with a(0) = 1, a(1) = -1, a(2) =
 *            -3, a(3) = -2
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 35, i64 n <= 70, i128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth term of sequence A316087. */
seq_t a(idx_t n)
{
    /* Cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t size  = 0;   /* number of elements stored (cache[0]..cache[size-1]) */

    /* If the requested term is already cached, return it. */
    if (n < size)
        return cache[n];

    /* Grow the cache if needed.  Note: idx_t is unsigned. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL)
        return (seq_t)0;   /* allocation failure – caller should handle */
    cache = tmp;
    size = (idx_t)new_size;

    /* Initialise the first four terms (if they are part of the cache). */
    if (size >= 1) cache[0] = (seq_t)1;
    if (size >= 2) cache[1] = (seq_t)-1;
    if (size >= 3) cache[2] = (seq_t)-3;
    if (size >= 4) cache[3] = (seq_t)-2;

    /* Compute remaining terms via the linear recurrence
       a(n) = 2*a(n-1) - 4*a(n-2) + a(n-3)  for n >= 4. */
    if (n >= 4)
    {
        for (idx_t i = 4; i <= n; ++i)
        {
            cache[i] = (seq_t)2 * cache[i - 1]
                     - (seq_t)4 * cache[i - 2]
                     +          cache[i - 3];
        }
    }

    return cache[n];
}
