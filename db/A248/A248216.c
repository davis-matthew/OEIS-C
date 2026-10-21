/*
 * A248216  a(n) = 6^n - 2^n.
 * https://oeis.org/A248216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - 2^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 4, a(n) = 8*a(n-1) - 12*a(n-2) for n >= 2 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)4;

    seq_t a_n_minus2 = (seq_t)0; /* a(0) */
    seq_t a_n_minus1 = (seq_t)4; /* a(1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i) {
        a_n = (seq_t)8 * a_n_minus1 - (seq_t)12 * a_n_minus2;
        a_n_minus2 = a_n_minus1;
        a_n_minus1 = a_n;
    }

    return a_n_minus1;
}
