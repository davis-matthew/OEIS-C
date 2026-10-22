/*
 * A321562  a(n) = Sum_{d divides n} (-1)^(d + n/d) * d^6.
 * https://oeis.org/A321562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d + n/d) * d^6
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 36, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow6(idx_t x)
{
    seq_t r = (seq_t)x;
    for (int i = 0; i < 5; ++i)
        r *= (seq_t)x;
    return r;
}
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d)
    {
        if (n % d != 0)
            continue;
        idx_t e = n / d;
        int sign = ((d + e) & 1) ? -1 : 1;
        sum += (seq_t)sign * pow6(d);
    }
    return sum;
}
