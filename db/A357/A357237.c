/*
 * A357237  Number of compositions (ordered partitions) of n into distinct
 *          parts of the form 2^j - 1.
 * https://oeis.org/A357237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ (k)! over all subsets S of distinct Mersenne numbers
 *            (2^j‑1) whose sum is n where k = |S| is the number of parts in
 *            the subset. That is the number of compositions (ordered
 *            partitions) of n into distinct parts of the form 2^j−1
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
/* Computes a(n) for any n ≥ 0.
   The function works for any build where seq_t and idx_t are defined
   externally.  No external libraries are used. */
seq_t a(idx_t n)
{
    /* Special case for n = 0: the empty composition counts as 1. */
    if (n == ((idx_t)0))
        return (seq_t)1;

    /* Collect all Mersenne numbers ≤ n.  There can be at most 64 of them. */
    idx_t vals[65];
    idx_t m = 0;
    for (idx_t j = ((idx_t)1); j < ((idx_t)64); ++j) {
        idx_t v = ((idx_t)1 << j) - ((idx_t)1);
        if (v > n)
            break;
        vals[m++] = v;
    }

    /* If no Mersenne numbers are ≤ n, no composition exists (except n=0
       handled above), so result is 0. */
    if (m == ((idx_t)0))
        return (seq_t)0;

    seq_t result = (seq_t)0;
    uint64_t total_masks = ((uint64_t)1) << m; /* m ≤ 64 */

    for (uint64_t mask = ((uint64_t)1); mask < total_masks; ++mask) {
        idx_t sum = ((idx_t)0);
        idx_t parts = ((idx_t)0);

        for (idx_t bit = ((idx_t)0); bit < m; ++bit) {
            if (mask & (((uint64_t)1) << bit)) {
                sum += vals[bit];
                ++parts;
            }
        }

        if (sum == n) {
            seq_t fact = (seq_t)1;
            for (idx_t i = ((idx_t)2); i <= parts; ++i)
                fact = fact * (seq_t)i; /* compute part! */
            result = result + fact;
        }
    }

    return result;
}
