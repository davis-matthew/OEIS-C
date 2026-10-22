/*
 * A374754  a(n) is the difference between the sum of the squares and the sum
 *          of the cubes for the n first terms of A002760.
 * https://oeis.org/A374754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 60, i32 n <= 2041, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A374754: a(n) is the difference between the sum of the squares
 * and the sum of the cubes for the first n terms of A002760 (the union
 * of squares and cubes, counting 0).  The recurrence is  
 *   a(1) = 0
 *   a(n) = a(n-1) + f * A002760(n)
 * where f = +1 if A002760(n) is a square but not a cube,
 *          f = -1 if it is a cube but not a square,
 *          f = 0 if it is both a square and a cube.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset – the first term is at index 1. */
    if (n == 0)
        return (seq_t)0;

    /* Static memoisation state. */
    static seq_t *cache = NULL;   /* cached a(1)…a(size) */
    static idx_t  size   = 0;     /* number of cached terms   */
    static idx_t  cap    = 0;     /* capacity of cache        */
    static idx_t  sq_i   = 0;     /* index for squares (i^2)  */
    static idx_t  cu_i   = 0;     /* index for cubes  (j^3)  */
    static seq_t  acc    = 0;     /* cumulative a(n) so far  */

    /* Grow the cache if necessary. */
    while (size < n)
    {
        if (cap == 0 || cap < n)
        {
            idx_t new_cap = cap ? cap << 1 : 16;
            if (new_cap < n)
                new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;   /* memory error: return 0 as safe fallback */
            cache = tmp;
            cap   = new_cap;
        }

        /* Compute the next element of A002760.  The sequence is the
         * sorted union of squares and cubes, starting with 0.  We
         * generate the next smallest value of either i^2 or j^3,
         * incrementing the corresponding index.  If both are equal,
         * advance both indices to avoid duplication.  */
        seq_t next_square = (seq_t)sq_i * sq_i;      /* i^2 */
        seq_t next_cube   = (seq_t)cu_i * cu_i * cu_i;/* j^3 */

        seq_t value;
        int   f;   /* +1, -1, or 0 */

        if (next_square < next_cube)
        {
            value = next_square;
            f     = +1;          /* square but not cube */
            ++sq_i;
        }
        else if (next_cube < next_square)
        {
            value = next_cube;
            f     = -1;          /* cube but not square */
            ++cu_i;
        }
        else   /* both equal (both square and cube) */
        {
            value = next_square; /* value is both square and cube */
            f     = 0;
            ++sq_i;
            ++cu_i;
        }

        /* Update the running sum a(n). */
        if (f == +1)
            acc += value;
        else if (f == -1)
            acc -= value;
        /* f == 0 leaves acc unchanged */

        /* Store the computed a(n) into the cache.  The first stored
         * value corresponds to a(1). */
        cache[size++] = acc;
    }

    return cache[n - 1];
}
