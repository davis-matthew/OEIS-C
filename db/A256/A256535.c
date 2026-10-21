/*
 * A256535  The largest number of T-tetrominoes that fit within an n X n
 *          square.
 * https://oeis.org/A256535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * For n ≥ 1: * if n ≤ 2: a(n) = 0 * else: * let m = n / 4, r = n
 *            % 4 * if r == 0: a(n) = 4*m*m * if r == 1: a(n) = 4*m*m + 2*m –
 *            1 * if r == 2: a(n) = 4*m*m + 4*m * if r == 3: a(n) = 4*m*m +
 *            6*m + 1
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;
    idx_t m = n / 4;
    idx_t r = n % 4;
    seq_t sm = (seq_t)m;
    switch (r)
    {
        case 0:
            return sm * sm * (seq_t)4;
        case 1:
            return sm * sm * (seq_t)4 + sm * (seq_t)2 - (seq_t)1;
        case 2:
            return sm * sm * (seq_t)4 + sm * (seq_t)4;
        case 3:
            return sm * sm * (seq_t)4 + sm * (seq_t)6 + (seq_t)1;
    }
    return (seq_t)0;  /* unreachable, but keeps compiler happy */
}
