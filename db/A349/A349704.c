/*
 * A349704  Maximum terminal Wiener index for a tree of n vertices.
 * https://oeis.org/A349704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 0, a(2) = 1; for 3 <= n <= 9: a(n) =
 *            (n-1)*(n-2); for n >= 10: let s = n/3 if n % 3 == 0: a(n) = s^3
 *            + 3*s^2 + s - 1 if n % 3 == 1: a(n) = s^3 + 4*s^2 + 3*s if n % 3
 *            == 2: a(n) = s^3 + 5*s^2 + 6*s + 2
 * Range:     u8 n <= 16, u16 n <= 118, u32 n <= 4873, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n <= 9) {
        seq_t nseq = (seq_t)n;
        return (nseq - 1) * (nseq - 2);
    }

    idx_t s_idx = n / 3;
    seq_t s = (seq_t)s_idx;
    seq_t s2 = s * s;
    seq_t s3 = s2 * s;

    switch (n % 3) {
        case 0:
            return s3 + 3 * s2 + s - 1;
        case 1:
            return s3 + 4 * s2 + 3 * s;
        case 2:
            return s3 + 5 * s2 + 6 * s + 2;
    }
    /* unreachable */
    return 0;
}
