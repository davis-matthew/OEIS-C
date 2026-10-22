/*
 * A309800  Sum of the even parts appearing among the largest parts of the
 *          partitions of n into 4 parts.
 * https://oeis.org/A309800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum over partitions of n into 4 positive integers of the
 *            largest part when that part is even. The function enumerates
 *            partitions in non‑increasing order and efficiently sums the even
 *            largest parts
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;

    for (idx_t k = 1; k <= n / 4; ++k) {
        idx_t max_j = (n - k) / 3;
        for (idx_t j = k; j <= max_j; ++j) {
            idx_t i_min = j;
            idx_t i_max = (n - j - k) / 2;
            if (i_min > i_max)
                continue;                     /* no i values possible */

            /* Largest part when i = j (smallest i) and i = i_max (largest i) */
            seq_t a_start = (seq_t)n - ((seq_t)2 * (seq_t)j) - (seq_t)k;
            seq_t a_end   = (seq_t)n - (seq_t)i_max - (seq_t)j - (seq_t)k;

            /* Range [a_end, a_start] contains all possible largest parts
               for this (k, j).  Sum the even numbers in this range.   */
            seq_t low  = a_end;
            seq_t high = a_start;

            if (low <= high) {
                seq_t ev_low  = low + (low & 1);          /* first even >= low */
                seq_t ev_high = high - (high & 1);        /* last even <= high */

                if (ev_low <= ev_high) {
                    seq_t n_even = ((ev_high - ev_low) / 2) + 1;
                    seq_t sum_even = n_even * (ev_low + ev_high) / 2;
                    total += sum_even;
                }
            }
        }
    }
    return total;
}
