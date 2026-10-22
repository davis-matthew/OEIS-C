/*
 * A342802  Replace 2^k with (-3)^k in binary expansion of n.
 * https://oeis.org/A342802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, easy, base
 * Formula:   a(n) = Σ_{k such that bit_k(n)=1} (-3)^k
 * Range:     i8 n <= 31, i16 n <= 1023, i32 n <= 9999, i64 n <= 9999,
 *            i128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t pow = 1;
    idx_t m = n;
    while (m)
    {
        if (m & 1u) res += pow;
        pow *= -(seq_t)3;
        m >>= 1;
    }
    return res;
}
