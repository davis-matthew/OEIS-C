/*
 * A278129  a(n) = 348*2^n - 188.
 * https://oeis.org/A278129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 348*2^n - 188; also satisfies a(0)=160, a(1)=508 * and
 *            a(n) = 3*a(n-1) - 2*a(n-2) for n ≥ 2
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 23, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)160;
    if (n == 1)
        return (seq_t)508;

    seq_t a0 = (seq_t)160;
    seq_t a1 = (seq_t)508;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = a1 * (seq_t)3 - a0 * (seq_t)2;
        a0 = a1;
        a1 = a2;
    }

    return a1;
}
