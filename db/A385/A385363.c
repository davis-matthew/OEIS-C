/*
 * A385363  Consecutive states of the linear congruential pseudo-random number
 *          generator (3613*s + 45289) mod 214326 when started at s=1.
 * https://oeis.org/A385363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3613*a(n-1)+45289) % 214326, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t seq[214327] = {0};
    static idx_t max_computed = 0;

    if (max_computed == 0) {
        seq[1] = (seq_t)1;
        max_computed = 1;
    }

    if (n < 1) return 0; /* undefined for indices below the offset */

    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t prev = seq[i - 1];
            seq[i] = ((seq_t)3613 * prev + (seq_t)45289) % (seq_t)214326;
        }
        max_computed = n;
    }

    return seq[n];
}
