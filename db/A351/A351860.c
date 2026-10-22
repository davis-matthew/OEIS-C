/*
 * A351860  Number of minimum cyclic edge cuts in the complete graph K_n.
 * https://oeis.org/A351860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n < 6, a(6) = 10, a(n) = n*(n-1)*(n-2)/6 for n >= 7
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;
    if (n == 6)
        return 10;
    seq_t tn = (seq_t)n;
    seq_t t = tn * (seq_t)(n - 1);
    t = t * (seq_t)(n - 2);
    t /= 6;
    return t;
}
