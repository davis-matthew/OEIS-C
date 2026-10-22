/*
 * A306504  Expansion of 1/(x^6+2*x^5-x^4-4*x^3-x^2-2*x+1).
 * https://oeis.org/A306504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 4*a(n-3) + a(n-4) - 2*a(n-5) -
 *            a(n-6), with initial terms
 *            a(0)=1,a(1)=2,a(2)=5,a(3)=16,a(4)=46,a(5)=128
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[6] = { (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)16, (seq_t)46, (seq_t)128 };
    if (n < 6)
        return base[n];

    seq_t buf[6];
    for (idx_t i = 0; i < 6; ++i)
        buf[i] = base[i];

    for (idx_t k = 6; k <= n; ++k) {
        seq_t next = 2*buf[5];
        next += buf[4];
        next += 4*buf[3];
        next += buf[2];
        next -= 2*buf[1];
        next -= buf[0];

        /* shift left: buf[0]..buf[4] become buf[1]..buf[5] */
        for (idx_t i = 0; i < 5; ++i)
            buf[i] = buf[i+1];
        buf[5] = next;
    }
    return buf[5];
}
