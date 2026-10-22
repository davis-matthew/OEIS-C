/*
 * A358691  Gilbreath transform of primes p(2k-1); see Comments.
 * https://oeis.org/A358691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(1)=a(2)=a(3)=a(4)=3, a(n)=1 for n ≥ 5)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)4)
        return (seq_t)3;
    else
        return (seq_t)1;
}
