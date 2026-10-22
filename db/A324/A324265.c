/*
 * A324265  a(n) = 5*343^n.
 * https://oeis.org/A324265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*343^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A324265 : a(n) = 5*343^n ;  a(0) = 5 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)5;         /* a(0) = 5                         */
    const seq_t mult = (seq_t)343;  /* common ratio                      */

    for (idx_t i = 0; i < n; ++i)
    {
        result *= mult;  /* repeated multiplication implements 343^n */
    }

    return result;
}
