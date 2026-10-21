/*
 * A252986  Numbers n such that the heptagonal number H(n) is equal to the sum
 *          of the hexagonal numbers X(m) and X(m+1) for some m.
 * https://oeis.org/A252986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+1442*a(n-2)-1442*a(n-3)-a(n-4)+a(n-5)
   with a(1)=2, a(2)=733, a(3)=2366, a(4)=1056553, a(5)=3411338. */

#include <stdlib.h>  /* malloc, realloc, abort, size_t */

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* 1‑based index, cache[0] unused */
    static size_t cap   = 0;      /* allocated size (number of entries) */
    static idx_t  max_n = 0;      /* highest index computed so far */

    if (n == 0)          /* undefined, return 0 */
        return (seq_t)0;

    /* Ensure cache can hold indices up to max(n,5) */
    idx_t needed = n > 5 ? n : 5;
    if (cap <= (size_t)needed) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= (size_t)needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize the first five terms if not already done */
    if (max_n < 5) {
        if (max_n < 1) cache[1] = (seq_t)2;
        if (max_n < 2) cache[2] = (seq_t)733;
        if (max_n < 3) cache[3] = (seq_t)2366;
        if (max_n < 4) cache[4] = (seq_t)1056553;
        if (max_n < 5) cache[5] = (seq_t)3411338;
        max_n = 5;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];

        cache[i] = t1 + (seq_t)1442 * t2
                     - (seq_t)1442 * t3
                     - t4 + t5;
    }
    max_n = n;
    return cache[n];
}
