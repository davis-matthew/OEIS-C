/*
 * A269272  Number of n X 4 0..3 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling three
 *          exactly once.
 * https://oeis.org/A269272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 72 * 49^(n-2) * (120*n-47) for n > 1 a(1) = 108
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)108;

    /* compute 49^(n-2) */
    idx_t exp = n - 2;
    seq_t base  = (seq_t)49;
    seq_t pow49 = (seq_t)1;
    while (exp) {
        if (exp & 1) pow49 *= base;
        base *= base;
        exp >>= 1;
    }

    seq_t factor = (seq_t)120 * (seq_t)n - (seq_t)47;
    return (seq_t)72 * pow49 * factor;
}
