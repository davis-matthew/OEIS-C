/*
 * A352505  Sum of all parts of all partitions of n into an even number of
 *          consecutive parts.
 * https://oeis.org/A352505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * #{m even | 2n/m is an odd integer and m*(m+1)/2 <= n}
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the sum of all parts of all partitions of n into an even number of consecutive parts   */

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of admissible even lengths m          */
    idx_t m     = 2;          /* start at the smallest even length           */
    for (;; m += 2) {
        /* stop when the smallest part would be < 1                */
        if (m * (m + 1) / 2 > n) break;

        /* 2n must be divisible by m and the result must be odd    */
        idx_t twice_n = 2 * n;
        if ((twice_n % m) == 0) {
            idx_t t = twice_n / m;   /* t = 2n / m  */
            if (t & 1)               /* odd ?       */
                ++count;
        }
    }
    return (seq_t)n * (seq_t)count;  /* a(n) = n * count                 */
}
