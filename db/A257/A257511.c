/*
 * A257511  Number of 1's in factorial base representation of n (A007623).
 * https://oeis.org/A257511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9624, u16 n <= 9624, u32 n <= 9624, u64 n <= 9624,
 *            u128 n <= 9624, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of 1's in the factorial base representation of n
 *
 * Implements: a(n) = count{ i ≥ 1 : digit_i(n) = 1 },
 * where digit_i(n) is obtained by repeatedly dividing n by (i+1) and
 * taking the remainder modulo (i+1).  This is the standard factorial
 * number system representation.
 */
seq_t a(idx_t n)
{
    seq_t count = 0;          /* number of 1's found */
    idx_t  x     = n;         /* working copy of n */

    for (idx_t i = 1; x > 0; ++i)
    {
        idx_t digit = x % (i + 1);
        if (digit == 1)
            count++;          /* count the 1's */
        x = x / (i + 1);
    }

    return count;
}
