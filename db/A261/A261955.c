/*
 * A261955  Start with a single equilateral triangle for n=0; for the odd n-th
 *          generation add a triangle at each expandable side of the triangles
 *          of the (n-1)-th generation (this is the "side to side" version);
 *          for the even n-th generation use the "vertex to side" version;
 *          a(n) is the number of triangles add...
 * https://oeis.org/A261955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Sequence A261955
 *   Recurrence:  a(0)=1, a(1)=3, a(2)=6, a(3)=15, a(4)=12, a(5)=24, a(6)=15,
 *                a(7)=33, a(8)=21, a(9)=45, a(10)=39, a(11)=72, a(12)=36, a(13)=78,
 *                a(n) = a(n-2) + a(n-8) - a(n-10)   for n ≥ 14
 */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache      = NULL;   /* array of computed values      */
    static size_t cap        = 0;      /* current capacity (bytes)      */
    static idx_t  computed_to = (idx_t)-1; /* highest index computed (incl.) */

    /* enlarge cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap << 1 : 1;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* initial terms of the recurrence (indices 0 … 13) */
    static const seq_t init[14] = {
        (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)15, (seq_t)12,
        (seq_t)24, (seq_t)15, (seq_t)33, (seq_t)21, (seq_t)45,
        (seq_t)39, (seq_t)72, (seq_t)36, (seq_t)78
    };

    /* compute missing terms up to n */
    idx_t start = computed_to + 1;      /* wraps to 0 when computed_to == (idx_t)-1 */
    for (idx_t i = start; i <= n; ++i) {
        if (i < 14) {
            cache[i] = init[i];
        } else {
            cache[i] = cache[i-2] + cache[i-8] - cache[i-10];
        }
    }
    computed_to = n;

    return cache[n];
}
