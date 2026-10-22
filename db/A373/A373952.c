/*
 * A373952  Number of integer compositions of n whose run-compression sums to
 *          3.
 * https://oeis.org/A373952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n % 3 == 0 && n >= 3 ? 1 : 0) + 2 * floor((n - 1) / 2)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t res = 0;
    if ((n % 3) == 0)
        res += 1;

    res += ((seq_t)(n - 1) / 2) * 2;
    return res;
}
