/*
 * A249580  List of quadruples (r,s,t,u): the matrix M =
 *          [[4,12,9][2,5,3][1,2,1]] is raised to successive negative powers,
 *          then (r,s,t,u) are the square roots of M[1,3], M[1,1], M[3,3],
 *          M[3,1] respectively.
 * https://oeis.org/A249580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 32, i32 n <= 72, i64 n <= 144,
 *            i128 n <= 292, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -3*a(n-4)+a(n-8) for n > 8,
   with a(1)=3, a(2)=-1, a(3)=-2, a(4)=1,
   a(5)=-9, a(6)=4, a(7)=7, a(8)=-3 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset is 1, ignore n = 0 */

    static seq_t *cache = NULL;          /* store computed terms, 0‑based */
    static idx_t cache_sz = 0;           /* number of terms currently stored */
    static idx_t cap = 0;                /* allocated capacity (number of slots) */

    /* Initialise cache with first 8 terms (index 0 → a(1)) */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-2;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)-9;
        cache[5] = (seq_t)4;
        cache[6] = (seq_t)7;
        cache[7] = (seq_t)-3;
        cache_sz = 8;
    }

    /* Grow the allocation if the requested index is beyond current capacity */
    if (n > cap) {
        idx_t new_cap = cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    if (n > cache_sz) {
        for (idx_t i = cache_sz + 1; i <= n; ++i) {
            /* i is 1‑based. a(i) = -3*a(i-4)+a(i-8) */
            cache[i-1] = (seq_t)(-3) * cache[i-5] + cache[i-9];
        }
        cache_sz = n;
    }

    return cache[n-1];
}
