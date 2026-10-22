/*
 * A363972  Expansion of Sum_{k>0} k^2 * x^(4*k-3) / (1 - x^(4*k-3)).
 * https://oeis.org/A363972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d==1 mod 4} ((d+3)/4)^2
 * Range:     u8 n <= 60, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t N = n;
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            idx_t d1 = d;
            if (d1 % 4 == 1) {
                seq_t t = ((seq_t)d1 + 3) / 4;
                sum += t * t;
            }
            idx_t d2 = N / d;
            if (d2 != d1 && d2 % 4 == 1) {
                seq_t t = ((seq_t)d2 + 3) / 4;
                sum += t * t;
            }
        }
    }
    return sum;
}
