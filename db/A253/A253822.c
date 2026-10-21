/*
 * A253822  Indices of centered triangular numbers (A005448) which are also
 *          octagonal numbers (A000567).
 * https://oeis.org/A253822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 1154*a(n-2) - 1154*a(n-3) - a(n-4) + a(n-5) with
 *            initial values: a(1)=1, a(2)=256, a(3)=833, a(4)=294848,
 *            a(5)=960705
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* a(0) is undefined, return 0 for safety */

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;

    /* Allocate or extend storage if necessary */
    if (cache == NULL) {
        cap = n > 6 ? n : 6;                       /* at least 6 slots for init */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                       /* allocation failure */
        /* initial known values */
        cache[0] = (seq_t)1;           /* a(1) */
        cache[1] = (seq_t)256;         /* a(2) */
        cache[2] = (seq_t)833;           /* a(3) */
        cache[3] = (seq_t)294848;        /* a(4) */
        cache[4] = (seq_t)960705;        /* a(5) */
        max_computed = 5;
    } else if (cap < (size_t)n) {
        size_t new_cap = n > cap * 2 ? n : cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                          /* reallocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms up to n */
    while (max_computed < n) {
        max_computed++;            /* now we compute a(max_computed) */
        if (max_computed <= 5)     /* first five values already set */
            continue;

        /* indices in 0‑based array */
        idx_t idx1 = max_computed - 2;  /* a(n-1)  */
        idx_t idx2 = max_computed - 3;  /* a(n-2)  */
        idx_t idx3 = max_computed - 4;  /* a(n-3)  */
        idx_t idx4 = max_computed - 5;  /* a(n-4)  */
        idx_t idx5 = max_computed - 6;  /* a(n-5)  */

        seq_t temp1 = cache[idx1];
        temp1 += (seq_t)1154 * cache[idx2];

        seq_t temp2 = (seq_t)1154 * cache[idx3];
        temp2 += cache[idx4];

        seq_t term = cache[idx5] + temp1 - temp2;
        cache[max_computed - 1] = term;
    }

    return cache[n - 1];
}
