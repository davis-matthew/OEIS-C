/*
 * A374843  Number of indices i in [n] such that in the trajectory of i for
 *          the Collatz (3x+1) problem the sum and the number of terms are
 *          coprime or the trajectory is not finite.
 * https://oeis.org/A374843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of i in [1..n] such that gcd(sum_i , len_i) = 1, where
 *            sum_i is the sum of the Collatz (3x+1) trajectory of i
 *            (including the starting i and the terminating 1), and len_i is
 *            the number of terms in that trajectory (including i and 1).
 *            Trajectories that do not finish are counted as 1
 * Range:     u8 n <= 10, u16 n <= 40, u32 n <= 9701, u64 n <= 9701,
 *            u128 n <= 9701, big any n
 * Helpers:   collatz_sum_len, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/collatz_sum_len.h"

seq_t a(idx_t n)
{
    seq_t acc = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum, len;
        collatz_sum_len(i, &sum, &len);
        if (gcd(sum, len) == 1)
            acc += 1;
    }
    return acc;
}
