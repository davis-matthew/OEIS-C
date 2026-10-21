/*
 * A284809  Write in base k, complement, reverse. Case k = 9.
 * https://oeis.org/A284809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse( complement base-9 representation of n )
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t  m = n;
    do
    {
        idx_t d = m % 9;                 /* current digit in base 9 */
        seq_t c = (seq_t)8 - (seq_t)d;   /* complement: 8 - d */
        result = result * 9 + c;         /* build the reversed number */
        m /= 9;
    } while (m > 0);
    return result;
}
