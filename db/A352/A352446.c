/*
 * A352446  Total number of parts in all partitions of n into an even number
 *          of consecutive parts.
 * https://oeis.org/A352446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m even} m, for each even m such that n - m*(m-1)/2
 *            is divisible by m and the resulting starting term is >= 1
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t m = 2; ; m += 2) {
        /* Compute m*(m-1)/2 safely in seq_t */
        seq_t t = (seq_t)m * (m - 1) / 2;
        if (t >= (seq_t)n) break;
        seq_t rem = (seq_t)n - t;
        if ((rem % (seq_t)m) == 0) {
            seq_t a_start = rem / (seq_t)m;
            if (a_start >= 1) total += m;
        }
    }
    return total;
}
