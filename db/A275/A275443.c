/*
 * A275443  Sum of the asymmetry degrees of all compositions of n without 2's.
 * https://oeis.org/A275443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 76, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A275443 - Sum of the asymmetry degrees of all compositions of n without 2's
   Recurrence (obtained from the rational generating function):
   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=4
   For n ≥ 6:
       a(n) = 3*a(n-1) - 2*a(n-2) + a(n-4) - 3*a(n-5) + a(n-6) - a(n-7)
   (with the convention a(k)=0 for k<0).
   The recurrence holds for all n≥6, thus the sequence can be computed in
   linear time with O(n) time and O(n) memory (only the last computed
   values are stored for subsequent calls). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq   = NULL;   /* stores a(k) values */
    static idx_t  last  = 5;      /* last computed index (a(5)) */
    /* initialise first six terms */
    if (seq == NULL) {
        /* allocate space for at least 6 terms plus possible larger n later */
        size_t cap = (n > 5U ? (size_t)n : 6U);
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (seq == NULL) return (seq_t)0;   /* allocation failure */
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)0;
        seq[2] = (seq_t)0;
        seq[3] = (seq_t)0;
        seq[4] = (seq_t)2;
        seq[5] = (seq_t)4;
        last = 5;
    }
    /* grow array if necessary */
    if (n > last) {
        size_t new_cap = (size_t)n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (seq == NULL) return (seq_t)0;   /* allocation failure */
        /* compute values from last+1 up to n */
        for (idx_t i = last + 1; i <= n; ++i) {
            /* positive part: 3*a(i-1) + a(i-4) + a(i-6) */
            seq_t pos = seq[i-1] * (seq_t)3 + seq[i-4] + seq[i-6];
            /* negative part: 2*a(i-2) + 3*a(i-5) + a(i-7) */
            seq_t neg = seq[i-2] * (seq_t)2 + seq[i-5] * (seq_t)3
                        + (i >= 7 ? seq[i-7] : (seq_t)0);
            seq[i] = pos - neg;          /* pos ≥ neg for all n */
        }
        last = n;
    }
    return seq[n];
}
