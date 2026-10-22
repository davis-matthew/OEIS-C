/*
 * A303066  Number of minimum and minimal total dominating sets in the
 *          n-Mycielski graph.
 * https://oeis.org/A303066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 0, a(2) = 1; for n >= 3, a(n) = a(n-1) * (7 * 2^(n-3) -
 *            2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 1;

    seq_t result = 1;   /* product of all terms */
    seq_t pow2   = 1;   /* 2^(i-3) for current i */

    for (idx_t i = 3; i <= n; ++i) {
        if (i > 3)       /* update pow2 for i-3 exponent */
            pow2 = pow2 * 2u;
        seq_t ratio = 7 * pow2 - 2;
        result = result * ratio;
    }
    return result;
}
