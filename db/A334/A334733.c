/*
 * A334733  a(n) is the sum of all parts of all partitions of n into
 *          consecutive parts that differ by 5.
 * https://oeis.org/A334733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A334541(n)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the sum of all parts of all partitions of n into consecutive parts that differ by 5.
   Since each partition sums to n, a(n) = (#partitions) * n. */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; unused but safe */
    idx_t k = 1;
    seq_t count = 0;

    while (1) {
        /* RHS = 5*k*(k-1)/2 + k   (max sum of the smallest possible partition with length k) */
        seq_t rhs = (seq_t)5 * k * (k - 1) / 2 + (seq_t)k;
        if (rhs > n) break;        /* longer partitions cannot exist */

        /* temp = n + 5*k*(k-1)/2   (k * x = n + 5*k*(k-1)/2) */
        seq_t temp = (seq_t)n + (seq_t)5 * k * (k - 1) / 2;
        if (temp % k == 0) {
            seq_t x = temp / k;    /* first part of the partition */
            /* last part = x - 5*(k-1) must be positive */
            if (x >= (seq_t)5 * (k - 1) + 1) {
                count += 1;
            }
        }
        k += 1;
    }
    return (seq_t)n * count;
}
