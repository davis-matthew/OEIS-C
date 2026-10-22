/*
 * A371532  Centered cuboctahedral numbers: the number of integer triples
 *          (x,y,z) such that max(|x|,|y|,|z|) <= n and |x|+|y|+|z| <= 2n.
 * https://oeis.org/A371532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (20*n^3 + 24*n^2 + 10*n + 3)/3
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 863, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long N = (unsigned long long)n;
    unsigned long long n2 = N * N;
    unsigned long long n3 = n2 * N;
    unsigned long long num = 20 * n3 + 24 * n2 + 10 * N + 3;
    return (seq_t)(num / 3);
}
