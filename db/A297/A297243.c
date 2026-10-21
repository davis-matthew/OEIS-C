/*
 * A297243  Down-variation of the base-15 digits of n; see Comments.
 * https://oeis.org/A297243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of all rises in the base‑15 digits of n (sum of (d_i
 *            – d_{i-1}) for each pair where d_i > d_{i-1}, proceeding from
 *            the least significant digit to the most significant)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset is 1, but handle 0 safely */
    seq_t sum = 0;
    idx_t prev = n % 15;                /* least significant digit */
    n /= 15;
    while (n) {
        idx_t curr = n % 15;            /* next significant digit */
        if (curr > prev) {
            sum += (seq_t)(curr - prev);
        }
        prev = curr;
        n /= 15;
    }
    return sum;
}
