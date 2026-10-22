/*
 * A347062  Record the number of zero terms having a following term, then the
 *          number of terms equal to 1 having a following term, then 2, 3,
 *          etc. until recording a zero, whereupon the count is reset.
 * https://oeis.org/A347062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence definition of A347062.
   Let m start at 0.  For each index n (n ≥ 0):
     a(n) = number of earlier indices j with j ≤ n-2 such that a(j) == m.
     If a(n) == 0, the next m is reset to 0; otherwise m is incremented by 1.
   This is exactly the rule described in the OEIS entry: count zero terms,
   then one terms, then two terms, etc., resetting whenever a zero is
   produced.  The algorithm below builds the sequence incrementally and
   returns a(n). */

#include <stdlib.h>   /* for malloc, calloc, free */

seq_t a(idx_t n)
{
    /* allocate array for a[0..n] */
    seq_t *a_arr = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!a_arr) return 0;                 /* in case of allocation failure */

    /* frequency table for values that are eligible to be counted.
       Size n+2 is sufficient because the largest value seen will never
       exceed n (in practice it is much smaller). */
    idx_t *freq = (idx_t*)calloc((size_t)(n + 2), sizeof(idx_t));
    if (!freq) { free(a_arr); return 0; }

    idx_t m = 0;                       /* current value to be counted */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* a(i) is the current frequency of m */
        a_arr[i] = (seq_t)freq[m];

        /* prepare m for next term */
        if (a_arr[i] == 0)
            m = 0;
        else
            ++m;

        /* make the term at index i-1 eligible for future counts */
        if (i > 0)
            ++freq[(size_t)a_arr[i - 1]];
    }

    seq_t result = a_arr[n];
    free(a_arr);
    free(freq);
    return result;
}
