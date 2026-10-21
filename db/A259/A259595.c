/*
 * A259595  Numerators of the other-side convergents to sqrt(6).
 * https://oeis.org/A259595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 10*a(n-2) - a(n-4) for n > 3, with initial values a(0)=3,
 *            a(1)=7, a(2)=27, a(3)=71
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)27;
    if (n == 3) return (seq_t)71;

    seq_t buf[4];
    buf[0] = (seq_t)3;
    buf[1] = (seq_t)7;
    buf[2] = (seq_t)27;
    buf[3] = (seq_t)71;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)10 * buf[2] - buf[0];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = next;
    }
    return buf[3];
}
