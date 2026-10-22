/*
 * A376185  a(n) = denominator of the sum S(n) defined in A376062.
 * https://oeis.org/A376185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(1) = 12 Recurrence: a(n) = a(n-1) + a(n-1)^2 / 4 for n >= 2
 *            (This follows from 1/a(n) = 1/a(n-1) - 1/(4*A376062(n)) with
 *            A376062(n) = a(n)/a(n-1) and S(n) = 1 - 5/a(n).) The
 *            implementation uses only integer arithmetic. For n <= 6 the
 *            intermediate product a^2 fits inside 64 bits on the 64‑bit bu...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a = 12;
    if (n <= 1)
        return a;

    for (idx_t i = 2; i <= n; ++i) {
        a = a + (a * a) / 4;
    }

    return a;
}
