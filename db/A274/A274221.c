/*
 * A274221  List of quadruples: 3*n*(3*n-1), 3*n*(3*n+1), (3*n+1)^2,
 *          (3*n+2)^2.
 * https://oeis.org/A274221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4k) = 3k(3k-1) * a(4k+1) = 3k(3k+1) * a(4k+2) = (3k+1)^2 *
 *            a(4k+3) = (3k+2)^2 * Offset: 0
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = n / 4;
    unsigned r = (unsigned)(n % 4);
    seq_t kk   = (seq_t)k;
    seq_t t3   = (seq_t)3 * kk;
    switch (r) {
        case 0: return t3 * (t3 - 1);
        case 1: return t3 * (t3 + 1);
        case 2: {
            seq_t tmp = t3 + 1;
            return tmp * tmp;
        }
        case 3: {
            seq_t tmp = t3 + 2;
            return tmp * tmp;
        }
    }
    return 0; /* unreachable */
}
