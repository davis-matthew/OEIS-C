/*
 * A364036  a(0) = 0, a(1) = 0; for n > 1, a(n) is the number of pairs of
 *          consecutive terms prior to a(n-1) that sum to the same value as
 *          a(n-2) + a(n-1).
 * https://oeis.org/A364036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=0; for n>1,
   a(n) = |{k | 0 ≤ k ≤ n-3 , a(k)+a(k+1) == a(n-2)+a(n-1)}| */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* memoised terms */
    static idx_t len = 0;       /* number of stored terms */
    static size_t cap = 0;      /* allocation capacity */

    /* Grow storage if needed (C++ requires explicit cast of realloc). */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq)                     /* allocation failure → zero */
            return (seq_t)0;
        cap = new_cap;
    }

    /* Compute missing terms up to index n. */
    for (idx_t i = len; i <= n; ++i) {
        if (i <= 1) {
            seq[i] = 0;               /* initial conditions */
        } else {
            seq_t target = seq[i - 2] + seq[i - 1];
            seq_t count  = 0;
            if (i >= 3) {
                for (idx_t k = 0; k <= i - 3; ++k) {
                    if (seq[k] + seq[k + 1] == target)
                        ++count;
                }
            }
            seq[i] = count;
        }
    }
    len = n + 1;
    return seq[n];
}
