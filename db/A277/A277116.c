/*
 * A277116  Number of segments used to represent the number n on a 7-segment
 *          display: variant where digits 6, 7 and 9 use 6, 3 and 5 segments,
 *          respectively.
 * https://oeis.org/A277116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum of segment counts for each decimal digit of n, where
 *            the 7‑segment mapping is: 0→6, 1→2, 2→5, 3→5, 4→4, 5→5, 6→6,
 *            7→3, 8→7, 9→5
 * Range:     u8 n <= 9847, u16 n <= 9847, u32 n <= 9847, u64 n <= 9847,
 *            u128 n <= 9847, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t seg[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 5};
    seq_t sum = 0;
    if (n == 0) {
        return seg[0];
    }
    while (n > 0) {
        sum += seg[n % 10];
        n /= 10;
    }
    return sum;
}
