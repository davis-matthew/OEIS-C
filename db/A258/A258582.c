/*
 * A258582  a(n) = n*(2*n + 1)*(4*n + 1)/3.
 * https://oeis.org/A258582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n + 1)*(4*n + 1)/3
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t n_t = (seq_t)n;
    seq_t f1 = n_t;
    seq_t f2 = (seq_t)2 * n_t + 1;
    seq_t f3 = (seq_t)4 * n_t + 1;

    /* one of f1, f2 or f3 is divisible by 3, divide it early to keep
       intermediary values small and avoid truncation */
    if ((f1 % 3) == 0) {
        f1 /= 3;
    } else if ((f2 % 3) == 0) {
        f2 /= 3;
    } else {
        f3 /= 3;
    }

    return f1 * f2 * f3;
}
