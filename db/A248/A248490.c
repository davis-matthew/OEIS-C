/*
 * A248490  Number of length 1+5 0..n arrays with some three disjoint pairs in
 *          each consecutive six terms having the same sum.
 * https://oeis.org/A248490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-4) - 4*a(n-5) + a(n-6)
 *            a(1)=22, a(2)=183, a(3)=724, a(4)=2125, a(5)=4986, a(6)=10147;
 *            The sequence satisfies this linear recurrence for all n >= 7
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 154, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a dynamic cache so any order of calls is fast. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms; cache[i] holds a(i+1) */
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (!initialized) {
        /* initialize with the first 6 terms */
        cache_cap = 8; /* initial capacity larger than 6 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */

        cache[0] = (seq_t)22;   /* a(1) */
        cache[1] = (seq_t)183;  /* a(2) */
        cache[2] = (seq_t)724;  /* a(3) */
        cache[3] = (seq_t)2125; /* a(4) */
        cache[4] = (seq_t)4986; /* a(5) */
        cache[5] = (seq_t)10147;/* a(6) */
        cache_len = 6;
        initialized = true;
    }

    if (n == 0) return (seq_t)0; /* undefined for n<1, return 0 */

    /* Ensure cache is large enough */
    if (n > cache_len) {
        size_t need = (size_t)n;
        if (need > cache_cap) {
            size_t new_cap = cache_cap;
            if (new_cap == 0) new_cap = 8;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute missing terms up to n */
        for (size_t i = cache_len; i < need; ++i) {
            /* i corresponds to a(i+1) */
            /* recurrence: a(k) = 4*a(k-1) - 5*a(k-2) + 5*a(k-4) - 4*a(k-5) + a(k-6) */
            seq_t term = ((seq_t)4) * cache[i-1];   /* 4*a(k-1) */
            term -= ((seq_t)5) * cache[i-2];       /* -5*a(k-2) */
            term += ((seq_t)5) * cache[i-4];       /* +5*a(k-4) */
            term -= ((seq_t)4) * cache[i-5];       /* -4*a(k-5) */
            term += cache[i-6];                    /* + a(k-6) */
            cache[i] = term;
        }
        cache_len = need;
    }

    return cache[n-1];
}
