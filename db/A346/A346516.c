/*
 * A346516  A variant of Van Eck's sequence: For n >= 0, a(n+1) is the result
 *          of combining by XOR the numbers k such that k < n and a(k) = a(n).
 *          Start with a(0)=0.
 * https://oeis.org/A346516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 259, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Variant of Van Eck's sequence: a(0)=0;
   a(n+1) = XOR of all k < n such that a(k) = a(n)   */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* static storage used to memoise computed terms */
static seq_t *seq = NULL;    /* array holding the sequence */
static idx_t  size = 0;      /* number of terms stored so far   */
static idx_t  cap  = 0;      /* current allocated capacity      */

/* Ensure that the array can store indices up to 'required'.
   After this call, cap > required. */
static void
ensure_capacity(idx_t required)
{
    if (required < cap)          /* already sufficient     */
        return;

    idx_t newcap = (cap == 0) ? 4 : cap * 2;
    while (newcap <= required)
        newcap <<= 1;            /* grow until we exceed required */

    seq_t *tmp = seq_alloc_new_array((size_t)newcap);
    for (idx_t i = 0; i < size; ++i)  /* copy old data */
        tmp[i] = seq[i];

    if (seq != NULL)                /* free old storage     */
        seq_free_array(seq);

    seq = tmp;
    cap = newcap;
}

/* Compute a(n) for the Van Eck variant defined above. */
seq_t
a(idx_t n)
{
    /* initialise on the first call */
    if (size == 0) {
        ensure_capacity(0);
        seq[0] = (seq_t)0;   /* a(0) = 0 */
        size = 1;
    }

    /* extend memoised terms up to index n */
    while (size <= n) {
        /* make sure there is room for the new element */
        ensure_capacity(size);

        seq_t prev = seq[size - 1];  /* a(n) with n = size-1 */
        seq_t val  = (seq_t)0;       /* XOR accumulator        */

        /* XOR all earlier indices k < size-1 where a(k)=prev */
        for (idx_t k = 0; k + 1 < size; ++k) {
            if (seq[k] == prev)
                val ^= (seq_t)k;
        }
        seq[size] = val;   /* store new term a(size)   */
        ++size;
    }

    return seq[n];
}
