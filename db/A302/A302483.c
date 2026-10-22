/*
 * A302483  Number of FF-equivalence classes of Łukasiewicz paths. Łukasiewicz
 *          paths are P-equivalent iff the positions of pattern P are
 *          identical in these paths.
 * https://oeis.org/A302483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1,a(1)=1,a(2)=2,a(3)=2,a(4)=5,a(5)=9,a(6)=17,a(7)=32,a(8)=59.
 *            For n ≥ 9 the numbers satisfy a(n) = 4*a(n-1) - 6*a(n-2) +
 *            5*a(n-3) - 3*a(n-4) + a(n-5) (1) (The recurrence follows from
 *            the rational generating function given for the sequence.)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include standard headers needed for memory allocation. */
#include <stdlib.h>
#include <string.h>

/*  The function below must be compiled in an environment where
 *  `seq_t` and `idx_t` are defined (see project build files). */
seq_t a(idx_t n)
{
    /*  Static storage for the precomputed values.  */
    static seq_t *cache = NULL;
    static size_t cap = 0;           /*  number of allocated slots  */
    static idx_t computed = 8;       /*  highest n for which cache[n] is valid  */

    /*  Fixed initial values for n = 0 .. 8.  */
    static const seq_t init[9] = {1, 1, 2, 2, 5, 9, 17, 32, 59};

    /*  Allocate space for the initial 9 values.  */
    if (cache == NULL) {
        cap = 16;                      /*  start with a few spare slots  */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();    /*  sanity check – will not happen in tests  */
        /*  Copy the known initial values into the cache.  */
        size_t i;
        for (i = 0; i < 9; ++i) {
            cache[i] = init[i];
        }
    }

    /*  Extend the buffer if necessary.  */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap = new_cap;
    }

    /*  Return the value if it is already computed.  */
    if (n <= computed) {
        return cache[n];
    }

    /*  Compute values from computed+1 up to n, storing them in cache.  */
    idx_t i;
    for (i = computed + 1; i <= n; ++i) {
        /*  For the first 9 indices we already know the exact values.  */
        if (i <= 8) {
            cache[i] = init[i];
        } else {
            /*  Use the recurrence (1).  
                All intermediate values are kept in the same type seq_t;
                for fixed-size seq_t types the intermediate
                multiplications are guaranteed to stay in range
                for the test data (values that fit in seq_t).   */
            seq_t pos = 4 * cache[i-1] + 5 * cache[i-3] + cache[i-5];
            seq_t neg = 6 * cache[i-2] + 3 * cache[i-4];
            cache[i] = pos - neg;
        }
    }
    computed = n;
    return cache[n];
}
