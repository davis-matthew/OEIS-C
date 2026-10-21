/*
 * A258453  G.f.: Sum_{k>0} x^((k^2 + k)/2) / (1 + x^k).
 * https://oeis.org/A258453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k>0} x^((k^2 + k)/2) / (1 + x^k)
 * Range:     i8 n <= 161, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t res = 0;
    for (idx_t k = 1; ; ++k)
    {
        seq_t tk = ((seq_t)k * (k + 1)) / 2;
        if (tk > n) break;
        seq_t diff = (seq_t)n - tk;
        if (diff % (seq_t)k == 0)
        {
            seq_t m = diff / (seq_t)k;
            if ((m & 1) != 0)
                res -= 1;
            else
                res += 1;
        }
    }
    return res;
}
