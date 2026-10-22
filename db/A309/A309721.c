/*
 * A309721  Number of right angles between the segments that are turned "on"
 *          when representing n on a 7-segment (calculator) display.
 * https://oeis.org/A309721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n/10) + a(n%10) (n > 9). Base values: a(0)=4, a(1)=0,
 *            a(2)=4, a(3)=4, a(4)=3, a(5)=4, a(6)=6, a(7)=1, a(8)=8, a(9)=6
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[10] = {4, 0, 4, 4, 3, 4, 6, 1, 8, 6};
    seq_t res = 0;
    while (n >= 10U) {
        res += base[(unsigned)(n % 10U)];
        n /= 10U;
    }
    res += base[(unsigned)n];
    return res;
}
