/*
 * A254962  Indices of hexagonal numbers (A000384) that are also centered
 *          pentagonal numbers (A005891).
 * https://oeis.org/A254962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A254962
 *
 *  Formula: a(n) = a(n-1)+18*a(n-2)-18*a(n-3)-a(n-4)+a(n-5)
 *  Initial values: a(1)=1, a(2)=2, a(3)=12, a(4)=31, a(5)=211
 *
 *  Parameters:
 *      n   – index (1‑based, a(1)=1)
 *
 *  Returns:
 *      a(n)   – the n‑th term of the sequence
 *
 *  Note:  The function memoises computed values in a static array so
 *  that subsequent calls are O(1).  The array is grown with realloc
 *  when a larger n is requested.  All arithmetic is performed with
 *  seq_t, so the implementation works for unsigned integer types
 *  and for arbitrary‑precision integer types (e.g. boost::multiprecision::cpp_int).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache  = NULL;  /* cache[0] is unused */
    static size_t capacity = 0;   /* allocated size (including index 0) */
    static idx_t last    = 0;     /* largest index computed so far */

    if (n < 1) return (seq_t)0;   /* offset is 1; out of range returns 0 */

    /* compute required capacity (at least 6 to hold first five terms) */
    size_t needed = (size_t)((n > 5) ? n + 1 : 6);
    if (capacity < needed)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache)
        {
            /* allocation failed – abort the program as required by the
             * specification (no error handling is needed otherwise). */
            exit(1);
        }
        cache = new_cache;
        capacity = needed;
    }

    /* initialise the first five terms once */
    if (last < 5)
    {
        /* zero the unused entry for safety */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)31;
        cache[5] = (seq_t)211;
        last = 5;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        /* a(i) = a(i-1) + 18*a(i-2) - 18*a(i-3) - a(i-4) + a(i-5) */
        seq_t t = cache[i - 1];
        t += (seq_t)18 * cache[i - 2];
        t -= (seq_t)18 * cache[i - 3];
        t -= cache[i - 4];
        t += cache[i - 5];
        cache[i] = t;
    }
    last = n;

    return cache[n];
}
