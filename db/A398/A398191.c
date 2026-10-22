/*
 * A398191  a(n) = n! * Sum_{j=0..n} (n + 4)^j / j!. Column 4 of A398187.
 * https://oeis.org/A398191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{j=0..n} (n + 4)^j / j!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= i;

    seq_t sum  = 0;
    seq_t term = fact;          /* j = 0 term */
    sum += term;

    for (idx_t j = 0; j < n; ++j) {
        term = term * (seq_t)(n + 4) / (seq_t)(j + 1);
        sum += term;
    }
    return sum;
}
