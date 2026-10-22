/*
 * A325103  Number of increasing pairs of positive integers up to n with no
 *          binary carries.
 * https://oeis.org/A325103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs {i,j} with 1 ≤ i < j ≤ n and (i
 *            & j) == 0 (i.e., the binary representations of i and j have no
 *            position with 1 in both)
 * Range:     u8 n <= 48, u16 n <= 1448, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i + 1; j <= n; ++j) {
            if ((i & j) == 0) {
                count++;
            }
        }
    }
    return count;
}
