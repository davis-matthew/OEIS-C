/*
 * A258009  If n <= 16, a(n) = n; for n > 16: a(16n + 6) = 16*n + 9, and for
 *          other cases with n > 16: a(2n) = 2*a(n), a(2n+1) = 2*a(n) + 1.
 * https://oeis.org/A258009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = - a(n) = n for 0 <= n <= 16 - if n % 16 == 6 (i.e. n ends
 *            in binary 0110), then a(n) = n + 3 - otherwise a(2k) = 2 * a(k)
 *            a(2k+1) = 2 * a(k) + 1 (This rewrites the right‑most “0110”
 *            substring to “1001”.)
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 16) return (seq_t)n;
    if ((n & 15) == 6)   /* n % 16 == 6 */
        return (seq_t)n + 3;
    if ((n & 1) == 0) {  /* even */
        seq_t v = a(n >> 1);
        return v << 1;
    } else {             /* odd */
        seq_t v = a(n >> 1);
        return (v << 1) | 1;
    }
}
