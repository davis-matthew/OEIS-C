/*
 * A303065  Number of numbers < n whose binary representation has the same
 *          difference between the numbers of 0's and 1's as n does.
 * https://oeis.org/A303065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 1648, u16 n <= 9977, u32 n <= 9977, u64 n <= 9977,
 *            u128 n <= 9977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A303065:  count of integers k < n whose binary representation
   has the same difference (ones – zeros) as that of n.
   The difference for 0 is defined to be -1. */

#include <stdint.h>
#include <stdbool.h>   /* for bool used by temp computations */

/* Return ones – zeros for x (with no leading zeros);
   and -1 for x == 0. */
static inline int diff_of(idx_t x)
{
    if (x == 0) return -1;

    /* bit length: position of most significant 1 + 1 */
    int bits = 64 - __builtin_clzll((unsigned long long)x);
    int ones = __builtin_popcountll((unsigned long long)x);
    return ones - (bits - ones);
}

/* Compute a(n).  For n == 0 the value is 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    int target_diff = diff_of(n);

    /* Count numbers i < n with diff_of(i) == target_diff. */
    seq_t count = 0;
    for (idx_t i = 0; i < n; ++i) {
        if (diff_of(i) == target_diff) {
            count++;
        }
    }
    return count;
}
