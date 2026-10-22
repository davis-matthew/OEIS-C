/*
 * A367286  Inverse of A342131.
 * https://oeis.org/A367286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(3*n) = 4*n * a(3*n+1) = 4*n + 2 * a(3*n+2) = 2*n + 1 *
 *            (Equivalently, A367286, the inverse of A342131.)
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 3;
    switch (n % 3) {
    case 0:
        return (seq_t)(4 * (seq_t)q);
    case 1:
        return (seq_t)(4 * (seq_t)q + 2);
    case 2:
        return (seq_t)(2 * (seq_t)q + 1);
    }
    /* unreachable */
    return 0;
}
