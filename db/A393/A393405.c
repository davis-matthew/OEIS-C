/*
 * A393405  Sum of the middle parts in the partitions of n into 3 parts whose
 *          smallest part is odd.
 * https://oeis.org/A393405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{odd j=1..⌊n/3⌋} Σ_{k=j..⌊(n‑j)/2⌋} k
 * Range:     u8 n <= 26, u16 n <= 176, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ans = 0;
    for (idx_t j = 1; j <= n/3; j += 2) {          /* odd j only            */
        idx_t m = (n - j) / 2;                    /* largest k for this j  */
        idx_t count = m - j + 1;                  /* number of k’s         */
        seq_t term = ((seq_t)(j + m)) * (seq_t)count / 2; /* sum of arithmetic seq */
        ans += term;
    }
    return ans;
}
