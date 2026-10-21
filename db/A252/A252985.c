/*
 * A252985  Numbers n such that the sum of the hexagonal numbers X(n) and
 *          X(n+1) is equal to the heptagonal number H(m) for some m.
 * https://oeis.org/A252985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+1442*a(n-2)-1442*a(n-3)-a(n-4)+a(n-5) Initial
 *            values: a(1)=1, a(2)=579, a(3)=1870, a(4)=835278, a(5)=2696899
 *            The function memoises the sequence so that arbitrary order
 *            requests for n ≥ 1 are served in O(1) after the first
 *            calculation
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1; we return 0 for n==0. */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;   /* array holding computed terms */
    static size_t  cap   = 0;     /* current allocation size   */
    static size_t  len   = 0;     /* number of terms stored    */

    /* initialise the first five terms */
    if (cache == NULL) {
        /* Allocate space for 5 terms. Use explicit cast for C++. */
        cache = (seq_t*)malloc(5 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation error */
        cache[0] = (seq_t)1;       /* a(1) */
        cache[1] = (seq_t)579;     /* a(2) */
        cache[2] = (seq_t)1870;    /* a(3) */
        cache[3] = (seq_t)835278;  /* a(4) */
        cache[4] = (seq_t)2696899; /* a(5) */
        cap   = 5;
        len   = 5;
    }

    size_t target = (size_t)n;            /* number of terms needed */

    /* Grow cache to contain all terms up to n. */
    if (target > len) {
        if (target > cap) {
            size_t new_cap = cap ? cap*2 : 8;
            while (new_cap < target)
                new_cap = new_cap*2;
            /* Reallocate with explicit cast. */
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* allocation error */
            cache = tmp;
            cap   = new_cap;
        }

        /* Compute missing terms using the recurrence. */
        for (size_t i = len; i < target; ++i) {
            seq_t t1 = cache[i-1];
            seq_t t2 = cache[i-2];
            seq_t t3 = cache[i-3];
            seq_t t4 = cache[i-4];
            seq_t t5 = cache[i-5];
            cache[i] = t1 + (seq_t)1442 * t2
                         - (seq_t)1442 * t3
                         - t4 + t5;
        }
        len = target;
    }

    return cache[n-1];
}
