/*
 * A282801  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 507", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=10, a(2)=111, a(3)=10, a(4)=11111, a(5)=1010; for
 *            n>5, a(n)=101*a(n-2)-100*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 10;
    if (n == 2) return 111;
    if (n == 3) return 10;
    if (n == 4) return 11111;
    if (n == 5) return 1010;

    /* circular buffer holding the last 6 values: indices 0..5 correspond to n mod 6 */
    seq_t buf[6] = {1, 10, 111, 10, 11111, 1010};

    for (idx_t i = 6; i <= n; ++i) {
        idx_t i2 = (i - 2) % 6;   /* a(i-2) */
        idx_t i4 = (i - 4) % 6;   /* a(i-4) */
        seq_t next = 101 * buf[i2] - 100 * buf[i4];
        buf[i % 6] = next;
    }
    return buf[n % 6];
}
