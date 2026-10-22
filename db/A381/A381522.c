/*
 * A381522  Sequence where k is appended after every k^2 occurrences of 1,
 *          with multiple values following a 1 listed in order.
 * https://oeis.org/A381522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A381522: For each 1 output, count the cumulative number of 1s seen.
   After a 1, if the cumulative total is divisible by k² (k ≥ 2), output k in
   ascending order.  This directly follows the definition. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe guard */

    idx_t count = 0;                /* terms produced so far */
    idx_t total_ones = 0;           /* cumulative number of 1s */

    while (1) {
        /* produce a 1 */
        ++count;
        if (count == n) return 1;
        ++total_ones;

        /* after this 1, output all k with total_ones divisible by k² */
        for (idx_t k = 2; total_ones / k >= k; ++k) {
            idx_t kk = k * k;       /* kk ≤ total_ones, safe */
            if (total_ones % kk == 0) {
                ++count;
                if (count == n) return (seq_t)k;
            }
        }
    }
}
