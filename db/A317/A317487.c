/*
 * A317487  Number of 4-cycles in the n-Bruhat graph.
 * https://oeis.org/A317487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!*(n-2)*(n-3)/8 for n > 1, a(n) = 0 for n <= 3
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) {
        return (seq_t)0;
    }

    /* compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* divide by 8 first to keep intermediates small */
    fact /= (seq_t)8;

    /* multiply by (n-2)*(n-3) */
    seq_t mul = (seq_t)(n - 2) * (seq_t)(n - 3);

    return fact * mul;
}
