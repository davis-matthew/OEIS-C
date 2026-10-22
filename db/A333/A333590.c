/*
 * A333590  a(n) = a(n-1) if half of the previous term pairs are inverted.
 *          a(n) = a(n-1) + 1 if more than half of the previous term pairs are
 *          inverted. a(n) = a(n-1) - 1 if fewer than half of the previous
 *          term pairs are inverted. a(1) = 0.
 * https://oeis.org/A333590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 0. For n > 1 let inv be the number of inversions among
 *            a(1)…a(n-1). Let max_inv = (n-1)(n-2)/2 be the maximum possible
 *            inversions for that length. If inv*2 > max_inv ⇒ a(n) = a(n-1) +
 *            1; If inv*2 < max_inv ⇒ a(n) = a(n-1) - 1; If inv*2 == max_inv ⇒
 *            a(n) = a(n-1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state: sequence cache, its length, allocated size, and
       current inversion count for the computed prefix */
    static seq_t *seq = NULL;      /* 1‑based indexing: seq[1] = a(1) */
    static size_t alloc = 0;       /* capacity of the array `seq` */
    static idx_t len = 0;          /* number of terms computed, len ≥ 0 */
    static idx_t inv = 0;          /* inversion count for prefix of length len */

    /* ensure capacity */
    if (seq == NULL) {
        alloc = 16;
        seq   = (seq_t *)malloc(alloc * sizeof(seq_t));
        if (seq == NULL) abort();    /* out of memory */
    }

    /* extend the cache up to n */
    while (len < n) {
        idx_t k = len + 1;                  /* index of the term to compute */

        seq_t prev = (k == 1) ? 0 : seq[k - 1];
        idx_t max_inv = len * (len - 1) / 2; /* (k-1)(k-2)/2 via len=k-1 */

        seq_t val;
        if (inv * 2 > max_inv)
            val = prev + 1;
        else if (inv * 2 < max_inv)
            val = prev - 1;
        else
            val = prev;

        /* grow array if needed */
        if ((size_t)k >= alloc) {
            alloc *= 2;
            seq = (seq_t *)realloc(seq, alloc * sizeof(seq_t));
            if (seq == NULL) abort();
        }

        seq[k] = val;

        /* update inversion count: count previous terms greater than new value */
        idx_t inc = 0;
        for (idx_t i = 1; i < k; ++i)
            if (seq[i] > val)
                ++inc;
        inv += inc;

        ++len;
    }

    return seq[n];
}
