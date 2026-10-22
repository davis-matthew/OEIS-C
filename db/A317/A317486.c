/*
 * A317486  Number of 6-cycles in the n-Bruhat graph.
 * https://oeis.org/A317486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * ( (n-2)/6 + (n-3)(n-4)/2 + (n-3)(n-4)(n-5)/3 ) for
 *            n>2; a(1) = 0, a(2) = 0
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    /* compute n! / 6  (safe because n! is divisible by 6 for n>2) */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;
    fact /= 6;

    /* numerator: (n-2) + 3*(n-3)*(n-4) + 2*(n-3)*(n-4)*(n-5) */
    seq_t term = (seq_t)(n - 2);

    if (n >= 4) {
        seq_t n3 = (seq_t)(n - 3);
        seq_t n4 = (seq_t)(n - 4);
        term += 3 * n3 * n4;
    }
    if (n >= 5) {
        seq_t n3 = (seq_t)(n - 3);
        seq_t n4 = (seq_t)(n - 4);
        seq_t n5 = (seq_t)(n - 5);
        term += 2 * n3 * n4 * n5;
    }

    return fact * term;
}
