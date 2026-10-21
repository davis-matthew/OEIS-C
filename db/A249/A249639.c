/*
 * A249639  Number of strings of length n over a 6 letter alphabet that begin
 *          with a nontrivial palindrome.
 * https://oeis.org/A249639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A249639
   a(0) = 0
   a(1) = 0
   For n ≥ 2:
      a(n) = 6*a(n‑1) + 6^ceil(n/2) – a(ceil(n/2))
   (indices as in OEIS, with a(0) the first term) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two initial terms directly.  The loop below
       starts at k = 2 so these cases must be returned before
       the array is allocated. */
    if (n == 0 || n == 1)
        return (seq_t)0;

    /* Allocate an array large enough to hold indices 0..n.  All
       elements are zero-initialised when the array is returned. */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (arr == NULL)
        return (seq_t)0;         /* allocation failure → return 0 */

    arr[0] = (seq_t)0;
    arr[1] = (seq_t)0;

    /* Keep track of 6^e for the current exponent e.  e starts at 0. */
    seq_t pow6 = (seq_t)1;   /* 6^0 */
    idx_t current_exp = 0;

    for (idx_t k = 2; k <= n; ++k)
    {
        /* ceil(k/2) is (k+1)/2 for integer k. */
        idx_t exp = (k + 1) / 2;

        /* Raise pow6 to the needed exponent.  Because exp increases by
           at most 1 every two steps, this loop executes a few times
           per iteration. */
        while (current_exp < exp)
        {
            pow6 = pow6 * (seq_t)6;
            ++current_exp;
        }

        /* The required previous value a(ceil(k/2)) is already stored
           in arr[exp] because exp ≤ k‑1. */
        arr[k] = 6 * arr[k - 1] + pow6 - arr[exp];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
