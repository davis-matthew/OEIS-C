/*
 * A243747  Expansion of (phi(q) - phi(q^2))^2 / 4 in powers of q where phi()
 *          is a Ramanujan theta function.
 * https://oeis.org/A243747
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{i+j=n} c(i)c(j) where c(k) = 1 if k is a perfect
 *            square, -1 if k is twice a perfect square, 0 otherwise. The
 *            sequence is produced by the convolution of this small integer
 *            sequence
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /*  The sequence is defined for n >= 2.  For n < 2
        the required value is undefined; return 0.  */
    if (n < 2) return (seq_t)0;

    /* Static cache of computed values.  All indices are
       stored from 0 up to the current maximum computed n.  */
    static seq_t *cache = NULL;          /* a[0..max_n]    */
    static int8_t *c = NULL;             /* c[0..max_n]    */
    static idx_t max_computed = 0;       /* highest n computed */

    /* Guarantee sufficient capacity.  (void*) casts are
       necessary for C++ builds where realloc returns void*. */
    idx_t needed = n + 1;                /* inclusive of n */
    if (needed > max_computed) {
        /* Reallocate arrays.  Use realloc on the old pointers. */
        seq_t *tmp_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp_cache) return (seq_t)0;   /* Allocation failed. */
        cache = tmp_cache;

        int8_t *tmp_c = (int8_t *)realloc(c, needed * sizeof(int8_t));
        if (!tmp_c) return (seq_t)0;
        c = tmp_c;

        /* Zero the new memory.  The template they provide for
           memory zeroing works for both C and C++. */
        for (idx_t i = max_computed; i < needed; ++i) {
            cache[i] = 0;
            c[i] = 0;
        }

        /* Fill the c array:  +1 for squares, -1 for twice squares. */
        for (idx_t m = 1; m * m <= n; ++m) {
            c[m * m] = 1;
        }
        for (idx_t t = 1; 2 * t * t <= n; ++t) {
            c[2 * t * t] = -1;    /* no overlap with pure squares */
        }

        /* Convolution:  a[i+j] += c[i]*c[j]  (i,j >= 1). */
        for (idx_t i = 1; i <= n; ++i) {
            if (c[i] == 0) continue;
            seq_t ci = (seq_t)c[i];
            for (idx_t j = 1; i + j <= n; ++j) {
                if (c[j] == 0) continue;
                cache[i + j] += ci * (seq_t)c[j];
            }
        }

        max_computed = n;
    }

    return cache[n];
}
