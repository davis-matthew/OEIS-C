/*
 * A309057  a(0) = 1; a(2*n) = 3*a(n), a(2*n+1) = a(n).
 * https://oeis.org/A309057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(number of 0's in the binary representation of n)
 * Range:     u8 n <= 63, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n=0 it gives a(0)=1.  For a(2*n)=3*a(n) and a(2*n+1)=a(n). */
seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    while (n > 0)
    {
        if ((n & 1ULL) == 0)
            res *= (seq_t)3;
        n >>= 1;
    }
    return res;
}
