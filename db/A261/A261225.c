/*
 * A261225  n minus the number of positive cubes needed to sum to n using the
 *          greedy algorithm: a(n) = n - A055401(n).
 * https://oeis.org/A261225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A055401(n) A055401(n) = number of cubes needed in the
 *            greedy representation of n
 * Range:     u8 n <= 266, u16 n <= 9664, u32 n <= 9664, u64 n <= 9664,
 *            u128 n <= 9664, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    idx_t orig = n;      /* keep the original value for the final subtraction */
    idx_t count = 0;     /* number of cubes used */

    while (n) {
        idx_t c = int_cube_root(n);      /* largest integer whose cube ≤ n */
        n -= c * c * c;                  /* subtract that largest cube */
        count++;                          /* one more cube used */
    }

    return (seq_t)orig - (seq_t)count;    /* result fits in seq_t */
}
