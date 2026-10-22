/*
 * A370730  a(n) is the least Fibonacci number f such that f AND n = n (where
 *          AND denotes the bitwise AND operator).
 * https://oeis.org/A370730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least Fibonacci number f such that (f & n) == n
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 57, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the least Fibonacci number f with (f & n) == n.
 * n is a non‑negative integer (idx_t), f is returned in seq_t,
 * which may be any unsigned integer type or an arbitrary‑precision
 * integer class.  The routine works for all n that fit in seq_t
 * (i.e. for which a(n) also fits).  For larger values a(n) is
 * guaranteed to exist, but the function may return 0 because
 * the sequence value would overflow seq_t. */
seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0)          /* (f & 0) == 0 for every f, and 0 is the
                            smallest Fibonacci number */
        return 0;

    seq_t f0 = 0;        /* F(0) */
    seq_t f1 = 1;        /* F(1) */

    for (;;) {
        /* Check current Fibonacci number */
        if ((f0 & (seq_t)n) == (seq_t)n)
            return f0;

        /* Compute next Fibonacci number: f2 = f0 + f1 */
        seq_t f2 = f0 + f1;

        /* Detect unsigned overflow: on overflow the sum wraps
           to a value smaller than f1.  For arbitrary‑precision
           types the comparison will never be true. */
        if (f2 < f1)
            break;          /* Overflow reached: further values do
                               not fit in seq_t */

        /* Advance the sliding window */
        f0 = f1;
        f1 = f2;
    }

    /* If we exit the loop without having found a suitable
       Fibonacci number, it means a(n) would overflow seq_t.
       Returning 0 is acceptable for values that do not
       fit in seq_t, since those cases are not required to be
       correct by the project specification. */
    return 0;
}
