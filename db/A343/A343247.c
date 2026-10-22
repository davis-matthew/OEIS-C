/*
 * A343247  Number of partitions of n into 3 parts where at least 1 part
 *          divides n.
 * https://oeis.org/A343247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions of n into 3 parts where at
 *            least one part divides n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;   /* no 3-part partition possible */

    seq_t count = 0;
    for (idx_t a = 1; a <= n / 3; ++a) {
        for (idx_t b = a; b <= (n - a) / 2; ++b) {
            idx_t c = n - a - b;          /* c >= b by loop bounds */
            if (n % a == 0 || n % b == 0 || n % c == 0)
                ++count;
        }
    }
    return count;
}
