/*
 * A297234  Down-variation of the base-12 digits of n; see Comments.
 * https://oeis.org/A297234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{i>0} max(0, d[i] - d[i-1]), where d[m]…d[0] are the
 *            base‑12 digits of n (d[0] least significant)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t prev = n % 12;
    n /= 12;
    while (n) {
        idx_t cur = n % 12;
        if (cur > prev) {
            sum += (seq_t)(cur - prev);
        }
        prev = cur;
        n /= 12;
    }
    return sum;
}
