/*
 * A354591  Numbers k that can be written as the sum of 4 divisors of k (not
 *          necessarily distinct).
 * https://oeis.org/A354591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest even number divisible by 3 or 4 or 5 (i.e.,
 *            numbers in the union of multiples of 4, 6, and 10)
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n >= 1 */
    seq_t next4 = 4;   /* multiples of 4 */
    seq_t next6 = 6;   /* multiples of 6 (even multiples of 3) */
    seq_t next10 = 10; /* multiples of 10 (even multiples of 5) */
    seq_t prev = 0;
    idx_t count = 0;

    while (1) {
        /* find the smallest current multiple */
        seq_t m = next4;
        if (next6 < m) m = next6;
        if (next10 < m) m = next10;

        /* skip duplicates and count unique terms */
        if (m != prev) {
            ++count;
            if (count == n) return m;
            prev = m;
        }

        /* advance all sequences that contributed the minimum */
        if (m == next4) next4 += 4;
        if (m == next6) next6 += 6;
        if (m == next10) next10 += 10;
    }
}
