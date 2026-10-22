/*
 * A303385  Total area of all rectangles with dimensions s and t such that s |
 *          t, n = s + t and s < t.
 * https://oeis.org/A303385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{s|n, 1 <= s < n/2} s*(n-s)
 * Range:     u8 n <= 23, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n < 3) {
        return 0;
    }
    idx_t limit = (n - 1) / 2;
    for (idx_t s = 1; s <= limit; ++s) {
        if (n % s == 0) {
            idx_t t = n - s;
            sum += (seq_t)s * (seq_t)t;
        }
    }
    return sum;
}
