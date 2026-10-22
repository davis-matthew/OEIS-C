/*
 * A385457  Number of odd entries in row n of the Fibonomial triangle
 *          (A010048).
 * https://oeis.org/A385457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + (n mod 3)) * 2^popcount(floor(n/3))
 * Range:     u8 n <= 381, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 3;           /* floor(n / 3) */
    unsigned int pc = 0;       /* popcount(m) */
    idx_t t = m;
    while (t) {
        t &= t - 1;            /* clear lowest set bit */
        pc++;
    }

    /* compute 2^pc, safe for any seq_t width */
    seq_t pow2 = 1;
    for (unsigned int i = 0; i < pc; ++i)
        pow2 <<= 1;

    seq_t factor = 1 + (seq_t)(n % 3);
    return pow2 * factor;
}
