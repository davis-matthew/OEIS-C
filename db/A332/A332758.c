/*
 * A332758  Number of fixed-point free involutions in the n-fold iterated
 *          wreath product of C_2.
 * https://oeis.org/A332758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 + 2^(2^(n-1)-1), a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* static cache for the sequence and auxiliary f(n)=2^(n-1)-1 */
static seq_t *a_arr     = NULL;   /* a values */
static seq_t *f_arr     = NULL;   /* f values */
static idx_t   cap      = 0;       /* capacity of arrays */
static idx_t   a_size   = 0;       /* number of computed a values */

/* Ensure a_arr and f_arr have capacity for indices up to need. */
static void
ensure_capacity(idx_t need)
{
    if (cap <= need)
    {
        idx_t new_cap = cap ? (cap << 1) : 1;
        while (new_cap <= need)
            new_cap <<= 1;

        seq_t *new_a = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (!new_a)
            exit(1);           /* allocation failure */
        seq_t *new_f = (seq_t *)realloc(f_arr, new_cap * sizeof(seq_t));
        if (!new_f)
            exit(1);

        a_arr = new_a;
        f_arr = new_f;
        cap   = new_cap;

        if (!a_size)          /* first time: initialise a[0] and f[0] */
        {
            a_arr[0] = (seq_t)0;
            f_arr[0] = (seq_t)0;
            a_size   = 1;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* a(0) is defined as 0 */

    ensure_capacity(n);

    /* If the value is already cached, return it. */
    if (n < a_size)
        return a_arr[n];

    /* Fill cache up to index n. */
    seq_t base2 = (seq_t)2;
    for (idx_t i = a_size; i <= n; ++i)
    {
        /* Compute f(i) = 2*f(i-1) + 1 , but f(1)=0. */
        if (i == 1)
            f_arr[i] = (seq_t)0;
        else
            f_arr[i] = f_arr[i - 1] * (seq_t)2 + (seq_t)1;

        /* term = 2^(f(i)) */
        seq_t term = big_pow(base2, f_arr[i]);

        /* a(i) = a(i-1)^2 + term */
        idx_t prev = i - 1;
        a_arr[i] = a_arr[prev] * a_arr[prev] + term;
    }

    a_size = n + 1;
    return a_arr[n];
}
