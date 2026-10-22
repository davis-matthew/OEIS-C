/*
 * A364268  a(n) = Sum_{k=1..n} k^2*sigma_2(k), where sigma_2 is A001157.
 * https://oeis.org/A364268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} k^2 * sigma_2(k) (sigma_2 is A001157)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

/* Sum_{k=1..n} k^2 * sigma_2(k) */
seq_t a(idx_t n)
{
    /* static cache: sigma_2 values and prefix sums */
    static seq_t *sigma2 = NULL;   /* sigma2[i] = sigma_2(i) */
    static seq_t *psum   = NULL;   /* psum[i] = Sum_{k=1..i} k^2*sigma_2(k) */
    static size_t capacity = 0;    /* current allocated size (indices 0..capacity-1) */

    /* need indices 0..n inclusive; if capacity insufficient, grow */
    size_t need = (size_t)n + 1;            /* reserve space for index 0..n */

    if (capacity < need) {
        size_t oldCap = capacity;
        /* grow capacity at least double or to satisfy need */
        capacity = oldCap ? oldCap * 2 : 1;
        while (capacity < need) capacity <<= 1;

        /* reallocate arrays to new capacity */
        sigma2 = (seq_t *)realloc((void *)sigma2, capacity * sizeof(seq_t));
        psum   = (seq_t *)realloc((void *)psum,   capacity * sizeof(seq_t));

        /* initialise new slots of sigma2 to 0 */
        for (size_t i = oldCap; i < capacity; ++i) sigma2[i] = 0;

        /* extend sigma_2 values only for the new indices */
        /* For each divisor d, add d^2 to all multiples m > oldCap-1 that
           are <= capacity-1. */
        for (size_t d = 1; d < capacity; ++d) {
            seq_t d2 = (seq_t)d * (seq_t)d;                     /* d^2 */
            /* first multiple not yet counted */
            size_t start = ((oldCap + d - 1) / d) * d;
            if (start < d) start = d;                          /* safety */
            for (size_t m = start; m < capacity; m += d) {
                sigma2[m] += d2;
            }
        }

        /* extend prefix sums for the new indices */
        for (size_t i = oldCap; i < capacity; ++i) {
            if (i == 0) {
                psum[0] = 0;
                continue;
            }
            seq_t term = (seq_t)i * (seq_t)i * sigma2[i];
            psum[i] = psum[i-1] + term;
        }
    }

    return psum[(size_t)n];
}
