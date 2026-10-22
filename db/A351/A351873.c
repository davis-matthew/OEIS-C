/*
 * A351873  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 3 or 4.
 * https://oeis.org/A351873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-5) + a(n-6) + 2*a(n-7) for n ≥ 7; initial
 *            values: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=12, a(5)=16,
 *            a(6)=21
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t base[7] = { (seq_t)1, (seq_t)2, (seq_t)4,
                                   (seq_t)8, (seq_t)12, (seq_t)16, (seq_t)21 };

    if (n <= 6) return base[n];

    /* Circular buffer holding the last 7 computed terms. */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i) buf[i] = base[i];

    for (idx_t i = 7; i <= n; ++i) {
        seq_t val = buf[(i-1) % 7] + buf[(i-5) % 7] + buf[(i-6) % 7]
                    + ((seq_t)2 * buf[(i-7) % 7]);
        buf[i % 7] = val;
    }
    return buf[n % 7];
}
