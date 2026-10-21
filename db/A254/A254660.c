/*
 * A254660  Numbers of words on alphabet {0,1,...,6} with no subwords ii,
 *          where i is from {0,1,...,4}.
 * https://oeis.org/A254660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 2*a(n-2), a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t prev2 = (seq_t)1; /* a(n-2) */
    seq_t prev1 = (seq_t)7; /* a(n-1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i) {
        current = prev1 * (seq_t)6 + prev2 * (seq_t)2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
