/*
 * A274089  Numbers repeated except that powers of 2 only appear once.
 * https://oeis.org/A274089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(A057716(n)/2)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 1;
    idx_t cnt = 0;
    while (cnt < n) {
        idx_t mult = 2;
        if ((v & (v - 1)) == 0)   /* v is a power of two */
            mult = 1;
        if (n <= cnt + mult)
            return v;
        cnt += mult;
        v++;
    }
    return v;   /* unreachable, kept to satisfy compiler */
}
