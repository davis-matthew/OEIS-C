/*
 * A351798  a(0) = 1; a(n) = (1/2) * Sum_{k=0..n-1} (k+1) * (k+2) * a(k) *
 *          a(n-k-1).
 * https://oeis.org/A351798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (1/2) * Σ_{k=0}^{n-1} (k+1)(k+2) * a(k) *
 *            a(n-1-k) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of sequence A351798.  The result is
   returned as type seq_t.  The function is fully self‑contained and
   does not use any static storage; it allocates an array of size n+1,
   builds the values iteratively and frees the memory before
   returning.  No external functions or headers other than stdlib.h
   are required.  */
seq_t a(idx_t n)
{
    /* n = 0 is the only special case.  */
    if (n == 0)
        return 1;

    /* Allocate workspace for a(0)…a(n).  */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr)
        exit(EXIT_FAILURE);  /* out of memory */

    arr[0] = 1;

    /* Build the sequence up to a(n).  */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k)
        {
            /* (k+1)(k+2) is computed in seq_t to avoid overflow of
               intermediate uint64_t values.  */
            seq_t w = (seq_t)(k + 1) * (seq_t)(k + 2);
            sum += w * arr[k] * arr[i - 1 - k];
        }
        arr[i] = sum / 2; /* the recurrence factor ½ */
    }

    seq_t result = arr[n];

    free(arr);
    return result;
}
