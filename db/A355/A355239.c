/*
 * A355239  Starting values k > 4 of a Collatz iteration reaching either k-1
 *          or k+1.
 * https://oeis.org/A355239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the nth smallest integer k > 4 such that the Collatz
 * iteration starting at k eventually reaches k-1 or k+1.
 * Offset: 1
 *
 * The routine computes candidates on demand, caching them in a static
 * array.  It is independent of call order and works for every
 * seq_t/idx_t combination provided by the build.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper functions supplied by the build system.  They respect the
 * characteristics of the build's arbitrary‑precision integer type. */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t n)   */
#include "../utils/free_seq_array.h"    /* free_seq_array(seq_t *p)    */

seq_t a(idx_t n)
{
    /* Static tables for caching the sequence values. */
    static seq_t *values = NULL;
    static size_t  len   = 0;   /* current number of stored terms   */
    static size_t  cap   = 0;   /* current allocation capacity      */
    static idx_t   next_k = 5;  /* next integer to test (k > 4)     */

    /* Ensure the table holds at least n terms. */
    while (len < n)
    {
        /* Current candidate k. */
        seq_t k       = (seq_t)next_k;
        seq_t x       = k;
        seq_t target1 = k - 1;
        seq_t target2 = k + 1;

        /* Run the Collatz iteration until we hit k-1, k+1, or 1. */
        while (x != target1 && x != target2 && x != 1)
        {
            if ((x & 1) == 0)        /* even */
                x >>= 1;
            else                     /* odd  */
                x = x * 3 + 1;
        }

        /* Store the value if it reaches k-1 or k+1. */
        if (x == target1 || x == target2)
        {
            /* Grow the array lazily. */
            if (len == cap)
            {
                size_t newcap = (cap == 0) ? 4 : cap * 2;
                seq_t *new_arr = alloc_seq_array(newcap);
                /* Copy old contents. */
                for (size_t i = 0; i < len; ++i)
                    new_arr[i] = values[i];
                /* Release old storage. */
                if (values != NULL)
                    free_seq_array(values);
                values = new_arr;
                cap    = newcap;
            }
            values[len++] = k;
        }

        /* Next candidate to test. */
        ++next_k;
    }

    /* The sequence is 1‑indexed. */
    return values[n - 1];
}
