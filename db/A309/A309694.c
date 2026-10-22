/*
 * A309694  Sum of the even parts appearing among the largest parts of the
 *          partitions of n into 3 parts.
 * https://oeis.org/A309694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=1..⌊n/3⌋} Σ_{i=j..⌊(n−j)/2⌋} (n−i−j) * ((n−i−j−1)
 *            mod 2)
 * Range:     u8 n <= 21, u16 n <= 137, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: add the largest part (n−i−j) whenever it is even. */
seq_t a(idx_t n)
{
    /* For n < 3 there are no partitions into 3 positive parts. */
    if (n < 3)
        return 0;

    seq_t sum = 0;

    /* Outer loop over the smallest part j (1 ≤ j ≤ ⌊n/3⌋). */
    for (idx_t j = 1; j <= n / 3; ++j) {
        /* For given j, the middle part i ranges from j to ⌊(n−j)/2⌋. */
        idx_t max_i = (n - j) / 2;
        for (idx_t i = j; i <= max_i; ++i) {
            idx_t e = n - i - j;              /* largest part */
            if ((e & 1) == 0)                /* e even? */
                sum += (seq_t)e;
        }
    }
    return sum;
}
