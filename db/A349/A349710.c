/*
 * A349710  Paschal full moon dates expressed as days after March 21 (Julian
 *          calendar).
 * https://oeis.org/A349710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (19*(n mod 19)+15) mod 30
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n % 19;
    seq_t res = ((seq_t)19 * (seq_t)m + (seq_t)15) % (seq_t)30;
    return res;
}
