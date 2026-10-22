/*
 * A359141  First differences of the pancake flipping (or sorting by prefix
 *          reversal) sequence A058986.
 * https://oeis.org/A359141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = 2 if (n+3) is a Fibonacci number (with index ≥5), 
   otherwise a(n) = 1.  The offset is 1 (a(1)=1). 
*/
seq_t a(idx_t n)
{
    if (n == 0)               /* below the offset: arbitrary behaviour */
        return (seq_t)0;

    /* target value to test for membership in the Fibonacci sequence */
    idx_t target = n + (idx_t)3;

    /* Compute Fibonacci numbers until we reach or surpass the target. */
    idx_t f1 = (idx_t)1;
    idx_t f2 = (idx_t)1;
    while (f2 < target) {
        idx_t f3 = f1 + f2;  /* f3 < 2*target, safe in idx_t (u64)         */
        f1 = f2;
        f2 = f3;
    }

    /* If the target equals a Fibonacci number, return 2; else 1. */
    if (f2 == target)
        return (seq_t)2;
    else
        return (seq_t)1;
}
