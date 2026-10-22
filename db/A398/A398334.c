/*
 * A398334  a(n) = (n - A004185(n)) / 9.
 * https://oeis.org/A398334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 902, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n - A004185(n)) / 9
 * where A004185(n) is the integer formed by sorting the decimal digits of n
 * in increasing order and removing any leading zeros (except when n = 0).
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count decimal digits of n */
    int cnt[10] = {0};
    idx_t tmp = n;
    if (tmp != 0) {
        while (tmp) {
            int d = tmp % 10;
            cnt[d]++;
            tmp /= 10;
        }
    }
    /* Construct A004185(n) by placing digits 1..9 in ascending order */
    seq_t sorted = 0;
    for (int d = 1; d <= 9; ++d) {
        int c = cnt[d];
        while (c--) {
            sorted = sorted * 10 + d;
        }
    }
    /* Compute (n - sorted) / 9 */
    seq_t diff = (seq_t)n - sorted;
    return diff / 9;
}
