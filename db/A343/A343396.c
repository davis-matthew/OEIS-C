/*
 * A343396  Number of partitions of n into 3 parts [r,s,t] such that r <= s <=
 *          t where at least 1 part does not divide any larger part.
 * https://oeis.org/A343396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 parts r≤s≤t such that at
 *            least one part does NOT divide any larger part (i.e. (s mod r ≠
 *            0) OR (t mod r ≠ 0) OR (t mod s ≠ 0))
 * Range:     u8 n <= 56, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* No partition into three parts is possible for n < 6. */
    if (n < 6) return 0;

    seq_t count = 0;

    /* Enumerate the smallest part r. */
    for (idx_t r = 1; r <= n / 3; ++r) {
        /* For a fixed r, the middle part s ranges from r up to (n‑r)/2
           so that t = n - r - s ≥ s. */
        idx_t max_s = (n - r) / 2;
        for (idx_t s = r; s <= max_s; ++s) {
            idx_t t = n - r - s;           /* t ≥ s by construction */

            /* Count this triple if it satisfies the condition. */
            if ((s % r) != 0 || (t % r) != 0 || (t % s) != 0) {
                ++count;
            }
        }
    }

    return count;
}
