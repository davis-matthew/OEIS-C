/*
 * A249041  Number of odd terms in first n terms of A249039.
 * https://oeis.org/A249041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=1; for n>2: a(n)=n - a(a(n-1)) - a(n-1-a(n-1))
 * Range:     u8 n <= 461, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Thread‑safe static state for memoization */
    static idx_t size = 0;          /* allocated length of array */
    static idx_t computed_to = 0;   /* largest index already computed */
    static seq_t *seq = NULL;       /* cache of a(k) values */

    /* Handle the offset: a(1)=1, a(2)=1; n==0 is undefined, return 0 */
    if (n < 1)
        return (seq_t)0;

    /* Ensure the cache is large enough for indices up to n */
    if (n + 1 > size)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_size * sizeof(seq_t));
        if (!tmp)
            exit(1);                   /* allocation failure */
        seq = tmp;
        size = new_size;

        /* (Re)initialize base cases if the array is newly created */
        if (size >= 3)
        {
            seq[0] = (seq_t)0;         /* unused, but harmless */
            seq[1] = (seq_t)1;
            seq[2] = (seq_t)1;
            computed_to = 2;
        }
    }

    /* Compute missing terms up to n */
    for (idx_t k = computed_to + 1; k <= n; ++k)
    {
        /* a(k-1) is guaranteed to be <= k-1, so fits in idx_t */
        idx_t a_prev = (idx_t)seq[k - 1];
        seq_t  term1 = seq[a_prev];            /* a(a(k-1)) */
        idx_t  idx2  = (k - 1) - a_prev;       /* k-1-a(k-1) */
        seq_t  term2 = seq[idx2];              /* a(k-1-a(k-1)) */
        seq[k] = (seq_t)k - term1 - term2;
    }
    computed_to = n;

    return seq[n];
}
