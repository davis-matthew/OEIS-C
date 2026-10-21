/*
 * A276059  Sum of the asymmetry degrees of all compositions of n with parts
 *          in {3,4,5,6, ...}.
 * https://oeis.org/A276059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 31, u32 n <= 58, u64 n <= 114,
 *            u128 n <= 228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses recurrence derived from the generating function:
 * a(0..6) = 0
 * a(n) = b(n)
 *        + 2*a(n-1) - a(n-3) - a(n-5)
 *        + 2*a(n-6) - a(n-8) - a(n-9) - a(n-10)
 * where b(n) =  2 if n==7,
 *              -2 if n==8,
 *               0 otherwise. */

#include <stddef.h>
#ifdef __cplusplus
#  include <cstdlib>
#  include <new>
#else
#  include <stdlib.h>
#endif

static seq_t *a_cache = NULL;
static idx_t a_cap   = 0;
static idx_t a_len   = 0;

/* Ensure that a_cache contains all values up to index n. */
static void a_ensure(idx_t n)
{
    if (n < a_len)
        return;

    /* Grow the array if necessary. */
    idx_t needed = n + 1;
    if (needed > a_cap) {
        idx_t newcap = a_cap ? a_cap * 2 : 16;
        while (newcap < needed)
            newcap <<= 1;

#ifdef __cplusplus
        seq_t *newptr = new seq_t[newcap];
#else
        seq_t *newptr = (seq_t *)malloc(newcap * sizeof(seq_t));
#endif
        /* copy existing values */
        for (idx_t i = 0; i < a_len; ++i)
            newptr[i] = a_cache[i];
#ifdef __cplusplus
        delete[] a_cache;
#else
        free(a_cache);
#endif
        a_cache = newptr;
        a_cap   = newcap;
    }

    /* Compute terms from a_len to n. */
    for (idx_t i = a_len; i <= n; ++i) {
        seq_t term = (seq_t)0;

        if (i >= 1)
            term += (seq_t)2 * a_cache[i - 1];

        if (i >= 3)
            term -= a_cache[i - 3];

        if (i >= 5)
            term -= a_cache[i - 5];

        if (i >= 6)
            term += (seq_t)2 * a_cache[i - 6];

        if (i >= 8)
            term -= a_cache[i - 8];

        if (i >= 9)
            term -= a_cache[i - 9];

        if (i >= 10)
            term -= a_cache[i - 10];

        if (i == 7)
            term += (seq_t)2;
        else if (i == 8)
            term -= (seq_t)2;

        a_cache[i] = term;
    }

    a_len = n + 1;
}

seq_t a(idx_t n)
{
    a_ensure(n);
    return a_cache[n];
}
