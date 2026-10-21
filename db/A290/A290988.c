/*
 * A290988  The arithmetic function v+-(n,3).
 * https://oeis.org/A290988
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/2 for even n a(n) = 2*floor((n-3)/6)+1 for odd n
 *            (offset 2: a(2)=1, a(3)=1, ...) The function works for all n ≥ 2
 *            and can be called in any order
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t for arithmetic in the result type. */
    seq_t tn = (seq_t)n;

    /* Even numbers: simple half */
    if ((n & 1) == 0)
        return tn / 2;

    /* Odd numbers: 2*floor((n-3)/6)+1 */
    /* Compute the floor division in the natural integer type to avoid
       overflow for very large n, then cast back to seq_t. */
    idx_t tsmall = (n - 3) / 6;
    return (seq_t)(2 * tsmall + 1);
}
