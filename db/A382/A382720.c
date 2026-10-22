/*
 * A382720  Number of entries in the n-th row of Pascal's triangle not
 *          divisible by 7.
 * https://oeis.org/A382720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏(d_i+1) for d_i the base‑7 digits of n (Lucas theorem
 *            for prime 7)
 * Range:     u8 n <= 292, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    while (n != 0) {
        unsigned d = (unsigned)(n % 7u);     /* digit in base 7 */
        res *= (seq_t)(d + 1);
        n /= 7u;
    }
    return res;
}
