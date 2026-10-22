/*
 * A349803  a(3*n) = 1 + 4*n^2, a(1+3*n) = 2 + 4*n*(n+1), a(2+3*n) = 5 +
 *          4*n*(n+1).
 * https://oeis.org/A349803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3*n) = 1 + 4*n^2, a(1+3*n) = 2 + 4*n*(n+1), a(2+3*n) = 5 +
 *            4*n*(n+1)
 * Range:     u8 n <= 23, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute integer division and remainder with idx_t (unsigned). */
    idx_t k = n / 3;
    idx_t r = n % 3;
    seq_t sk = (seq_t)k;   /* cast to seq_t for arithmetic */
    if (r == 0) {
        /* a(3*k) = 1 + 4*k^2 */
        return 1 + 4 * sk * sk;
    } else if (r == 1) {
        /* a(3*k+1) = 2 + 4*k*(k+1) */
        return 2 + 4 * sk * (sk + 1);
    } else { /* r == 2 */
        /* a(3*k+2) = 5 + 4*k*(k+1) */
        return 5 + 4 * sk * (sk + 1);
    }
}
