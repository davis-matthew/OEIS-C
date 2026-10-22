/*
 * A350093  a(n) = Sum_{k=0..n} n OR k where OR is the bitwise logical OR
 *          operator (A003986).
 * https://oeis.org/A350093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0)=0, a(1)=2 a(2*n) = 4*a(n) - n a(2*n+1) = 4*a(n) + 2*n + 2
 *            (valid for all n ≥ 0)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a_rec(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t m = n >> 1;                      // m = floor(n/2)
    if ((n & 1) == 0)                     // n is even
        return 4 * a_rec(m) - (seq_t)m;
    else                                   // n is odd
        return 4 * a_rec(m) + 2 * (seq_t)m + 2;
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
