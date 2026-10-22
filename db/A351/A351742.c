/*
 * A351742  Number of partitions of 2n into n parts of size 1, 5, 10 or 25.
 * https://oeis.org/A351742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of non‑negative integer solutions (b,c,d) of 4b +
 *            9c + 24d = n which is equivalent to counting partitions of 2n
 *            into n parts of sizes 1,5,10,25
 * Range:     u8 n <= 635, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;

    idx_t max_d = n / 24;                // maximum d such that 24d <= n
    for (idx_t d = 0; d <= max_d; ++d) {
        idx_t rem_d = n - d * 24;        // remaining sum after choosing d

        idx_t max_c = rem_d / 9;         // maximum c such that 9c <= rem_d
        for (idx_t c = 0; c <= max_c; ++c) {
            idx_t rem = rem_d - c * 9;  // remaining sum after choosing c

            /* rem must be divisible by 4 to allow integer b */
            if ((rem & 3) == 0) {        // same as rem % 4 == 0
                ++count;                // a valid (d, c, b) triple found
            }
        }
    }

    return count;
}
