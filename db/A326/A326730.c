/*
 * A326730  Number of iterations of A326729(x) starting at x = n to reach 0.
 * https://oeis.org/A326730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(2*n) = a(n) + 2*popcount(n), a(2*n+1) = a(n) + 1, a(0) = 0
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline unsigned int popcnt_u64(idx_t x) {
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n) {
    seq_t acc = 0;
    while (n) {
        if (n & 1) {                /* odd case: a = a(n/2) + 1 */
            acc += 1;
            n >>= 1;
        } else {                    /* even case: a = a(n/2) + 2*popcount(n/2) */
            idx_t m = n >> 1;
            acc += (seq_t)(2 * popcnt_u64(m));
            n = m;
        }
    }
    return acc;
}
