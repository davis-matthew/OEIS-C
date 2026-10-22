/*
 * A340391  Number of partitions of n into 4 parts such that the largest part
 *          is equal to the square of the smallest part.
 * https://oeis.org/A340391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 parts * with largest
 *            part equal to square of smallest part. * For each s = smallest
 *            part (s ≥ 1), the largest part is d = s². * The remaining sum
 *            rem = n – s – s² must be split into two parts * b and c with s ≤
 *            b ≤ c ≤ s² and b + c = rem. The code counts *...
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Iterate over possible smallest part s.  s² + s must not exceed n. */
    for (idx_t s = 1;; ++s) {
        seq_t sq = (seq_t)s * (seq_t)s;   /* s² */
        if (sq + s > n) break;            /* no larger s possible */

        /* Remaining sum for the two middle parts. */
        seq_t rem = (seq_t)n - sq - s;
        idx_t rem_d = (idx_t)rem;         /* safe: rem ≤ n */

        /* Need at least s + s = 2·s for the two middle parts. */
        if (rem_d < 2 * s) continue;

        /* Enumerate b.  Because b ≤ c and b + c = rem, we only need
         * to consider b from s up to floor(rem / 2). */
        idx_t b_end = rem_d / 2;
        for (idx_t b = s; b <= b_end; ++b) {
            idx_t c = rem_d - b; /* automatically c ≥ b */

            /* Largest part condition: c ≤ s². */
            if ((seq_t)c > sq) continue;

            ++count; /* valid (b, c) pair found */
        }
    }

    return count;
}
