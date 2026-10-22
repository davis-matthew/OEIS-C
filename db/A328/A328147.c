/*
 * A328147  a(n) = A025586(n)/4 for n>=3.
 * https://oeis.org/A328147
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A025586(n)/4 (maximum term of the Collatz sequence
 *            starting at n, divided by 4)
 * Range:     u8 n <= 26, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: for n >= 3 the maximum is always divisible by 4. */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type so arithmetic is performed
       using the appropriate width. */
    seq_t cur = (seq_t)n;
    seq_t max_val = cur;

    /* Iterate the Collatz map until reaching 1.  Track the maximum value seen. */
    while (cur > 1) {
        if (cur & 1)         /* odd case: 3*cur + 1 */
            cur = 3 * cur + 1;
        else                 /* even case: cur / 2 */
            cur = cur / 2;

        if (cur > max_val)
            max_val = cur;
    }

    /* Return the maximum divided by 4.  For n >= 3 the division is exact. */
    return max_val / (seq_t)4;
}
