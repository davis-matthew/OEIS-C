/*
 * A347173  Sum of squares of odd divisors of n that are <= sqrt(n).
 * https://oeis.org/A347173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d odd, d|n, d <= sqrt(n)} d^2
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* integer floor of sqrt(n) via binary search */
    idx_t low = 0;
    idx_t high = n;
    idx_t sqrt_n = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        /* avoid overflow: mid > n/mid <=> mid*mid > n */
        if (mid != 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            sqrt_n = mid;
            low = mid + 1;
        }
    }

    seq_t sum = 0;
    for (idx_t d = 1; d <= sqrt_n; d += 2) {
        if (n % d == 0) {
            seq_t dd = (seq_t)d;
            sum += dd * dd;
        }
    }
    return sum;
}
