/*
 * A363271  Vertical sum of n in base 10.
 * https://oeis.org/A363271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n/10) + (n%10 == 9 ? 10 : n%10)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = floor(n / 10) */
    idx_t k = n / 10;
    /* m = n mod 10 */
    idx_t m = n % 10;

    seq_t result = (seq_t)k;

    if (m == 9)
        result += (seq_t)10;
    else
        result += (seq_t)m;

    return result;
}
