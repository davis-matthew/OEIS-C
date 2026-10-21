/*
 * A279847  a(n) = Sum_{k=1..n} k^2*(floor(n/k) - 1).
 * https://oeis.org/A279847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} k^2 * (floor(n/k) - 1)
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    if (n == 0) return 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k - 1;          /* floor(n/k) - 1 */
        if (t == 0) continue;         /* term will be zero */
        unsigned __int128 temp = (unsigned __int128)k * k *
                                (unsigned __int128)t;
        sum += (seq_t)temp;
    }
    return sum;
}
