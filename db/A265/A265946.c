/*
 * A265946  Sums of positive differences and negated sums of negative
 *          differences in the Collatz trajectory of n.
 * https://oeis.org/A265946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 652, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A265946:
 * a(2k-1) = sum of positive differences in the Collatz trajectory of k;
 * a(2k)   = sum of absolute values of negative differences in the Collatz trajectory of k.
 * Here k = (n+1)/2 (integer division). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* below offset – undefined, return 0 */

    /* Determine the Collatz start number for this index. */
    idx_t k = (n + 1) / 2;

    /* Compute the Collatz trajectory for k and accumulate sums. */
    seq_t cur  = (seq_t)k;
    seq_t pos  = 0;                     /* sum of positive differences */
    seq_t neg  = 0;                     /* sum of absolute values of negative differences (positive number) */

    while (cur != (seq_t)1)
    {
        seq_t nxt;
        if (cur & (seq_t)1)              /* odd: 3x + 1 */
            nxt = cur * (seq_t)3 + (seq_t)1;
        else                            /* even: x / 2 */
            nxt = cur / (seq_t)2;

        if (nxt > cur)
            pos += nxt - cur;
        else
            neg += cur - nxt;           /* store the positive magnitude */

        cur = nxt;
    }

    /* Return the appropriate part of the pair. */
    if (n & (idx_t)1)
        return pos;                     /* odd index -> positive sum */
    else
        return neg;                     /* even index -> absolute negative sum */
}
