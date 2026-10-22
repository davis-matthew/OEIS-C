/*
 * A343399  Number of partitions of n into 3 distinct parts [r,s,t] such that
 *          r < s < t where at least 1 part does not divide any larger part.
 * https://oeis.org/A343399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 889, u32 n <= 5200, u64 n <= 5200,
 *            u128 n <= 5200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A343399.
   a(n) = number of partitions of n into three distinct parts  r < s < t
   such that not all of the following divisibilities hold:
       r divides s  AND  r divides t  AND  s divides t
   In other words, there must exist a part among r or s that does not
   divide every larger part.  The implementation enumerates all valid
   triples r < s < t and counts those that satisfy the condition. */

/* Requires that idx_t (uint64_t) and seq_t (suitable unsigned integer
   type) are defined by the build system. */

seq_t a(idx_t n)
{
    if (n < 6) return 0;          /* need at least 1+2+3=6 */

    seq_t count = 0;
    /* r must be at most (n-3)/3 to leave room for s and t */
    for (idx_t r = 1; r <= (n - 3) / 3; ++r) {
        /* s must be > r and < (n - r)/2 */
        idx_t s_max = (n - r - 1) / 2;
        for (idx_t s = r + 1; s <= s_max; ++s) {
            idx_t t = n - r - s;          /* automatically t > s */
            /* skip if r|s, r|t and s|t all hold */
            if ((s % r == 0) && (t % r == 0) && (t % s == 0)) {
                continue;
            }
            ++count;
        }
    }
    return count;
}
