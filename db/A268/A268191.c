/*
 * A268191  The difference between the two largest distinct parts of a
 *          partition (0 if no distinct parts), summed over all partitions of
 *          n.
 * https://oeis.org/A268191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=2..n} Sum_{j=1..i-1} (i-j) * Sum_{a>=1,
 *            n-a*i-j>=0} P_j(n-a*i-j) where P_j(m) is the number of
 *            partitions of m into parts <= j. This implementation builds the
 *            partition numbers P_j incrementally and sums the required
 *            contributions on the fly. It runs in O(n²) time and O(n) memory,
 *            s...
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 108, u64 n <= 372,
 *            u128 n <= 1357, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Allocate array for partitions into parts <= current j.
       p[s] holds P_j(s) for current j.         */
    seq_t *p = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!p) return (seq_t)0;   /* allocation failure */

    /* Initially j = 0: only one partition of 0, none of the others */
    p[0] = 1;

    seq_t result = 0;

    for (idx_t j = 1; j < n; ++j) {
        /* Update p to represent partitions into parts <= j.
           Unbounded coin change DP: add part j. */
        for (idx_t s = j; s <= n; ++s) {
            p[s] += p[s - j];
        }

        /* For all i > j, accumulate contributions to a(n). */
        idx_t max_i = n;
        for (idx_t i = j + 1; i <= max_i; ++i) {
            seq_t diff = (seq_t)(i - j);
            /* Number of times part i can appear at least once */
            idx_t max_a = (n - j) / i;
            for (idx_t a = 1; a <= max_a; ++a) {
                idx_t rem = n - a * i - j;
                result += diff * p[rem];
            }
        }
    }

    free(p);
    return result;
}
