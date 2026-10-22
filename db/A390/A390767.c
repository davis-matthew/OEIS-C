/*
 * A390767  Number of digits in base-9 representation of n.
 * https://oeis.org/A390767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(log_9(n)) + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    idx_t c = 0;
    while (n > 0) {
        n /= 9;
        ++c;
    }
    return (seq_t)c;
}
