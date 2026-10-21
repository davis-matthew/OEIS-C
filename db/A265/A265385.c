/*
 * A265385  Sequence defined by a(1)=a(2)=1 and a(n) = gray(a(n-1) + a(n-2)),
 *          with gray(m) = A003188(m).
 * https://oeis.org/A265385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A265385:
   a(1) = a(2) = 1;
   a(n) = gray(a(n-1)+a(n-2))
   with gray(m) = m ^ (m >> 1). */

/* No external allocation is required – the recurrence can be
   evaluated iteratively using only the two most recent values. */

#include <stddef.h>  /* for size_t, if needed */

/* Gray‑code mapping for an unsigned integer type.  Works for all sizes. */
static inline seq_t gray(seq_t m)
{
    return m ^ (m >> 1);
}

/* Return the n‑th term (1‑based).  n==0 is undefined for the sequence; we
   return 0 in that case as a harmless sentinel. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined; harmless */

    if (n == 1 || n == 2) return (seq_t)1; /* base values */

    seq_t prev2 = (seq_t)1;  /* a(1) */
    seq_t prev1 = (seq_t)1;  /* a(2) */
    seq_t cur;

    /* Compute a(3) .. a(n) iteratively */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t sum = prev1 + prev2;
        cur     = gray(sum);
        prev2   = prev1;
        prev1   = cur;
    }

    return prev1;   /* a(n) */
}
