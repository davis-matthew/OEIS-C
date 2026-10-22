/*
 * A322048  Final elements in rows when A322050 is displayed as a triangle.
 * https://oeis.org/A322048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*a(n-1) + 11 for n >= 5; initial values: a(1)=1, a(2)=5,
 *            a(3)=15, a(4)=35
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)15;
    if (n == 4) return (seq_t)35;
    /* n >= 5 */
    seq_t val = (seq_t)35;
    for (idx_t i = 5; i <= n; ++i) {
        val = val * (seq_t)2 + (seq_t)11;
    }
    return val;
}
