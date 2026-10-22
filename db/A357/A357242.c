/*
 * A357242  Number of n node tournaments that have exactly two circular
 *          triads.
 * https://oeis.org/A357242
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(n - 3 + (1/18)*(n - 4)*(n - 5))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    if (n >= 6) {
        fact /= 18;
        seq_t num = (seq_t)n * n + 9 * (seq_t)n - 34;
        return fact * num;
    } else {
        seq_t term2 = ((seq_t)(n - 4)) * (n - 5) / 18;
        return fact * ((seq_t)(n - 3) + term2);
    }
}
