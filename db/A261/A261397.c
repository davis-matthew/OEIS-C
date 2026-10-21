/*
 * A261397  a(n) = 3^n*Fibonacci(n).
 * https://oeis.org/A261397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=3, a(n)=3*a(n-1)+9*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 3;
    seq_t a0 = 0;
    seq_t a1 = 3;
    seq_t an = 0;
    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)3 * a1 + (seq_t)9 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
