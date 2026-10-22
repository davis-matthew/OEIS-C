/*
 * A357392  E.g.f. satisfies A(x) = -log(1 - x * exp(2 * A(x))).
 * https://oeis.org/A357392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3 * n - 1)! / (2 * n)! (n > 0, a(0) = 0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t result = 1;
    /* product of integers from 2*n + 1 to 3*n - 1 inclusive */
    for (idx_t k = 2 * n + 1; k <= 3 * n - 1; ++k) {
        result *= (seq_t)k;
    }
    return result;
}
