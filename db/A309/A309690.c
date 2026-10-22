/*
 * A309690  Sum of the even parts appearing among the second largest parts of
 *          the partitions of n into 3 parts.
 * https://oeis.org/A309690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..⌊n/3⌋} Sum_{i=j..⌊(n-j)/2⌋} i * ((i-1) mod 2)
 *            where ((i-1) mod 2) is 1 for even i and 0 for odd i. The
 *            implementation evaluates the inner sum using
 *            arithmetic‑progression arithmetic, yielding O(n) time
 * Range:     u8 n <= 28, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ans = 0;
    /* j runs from 1 to floor(n/3) */
    for (idx_t j = 1; j <= n / 3; ++j) {
        /* maximum i for this j */
        idx_t max_i = (n - j) / 2;
        idx_t start = j;

        /* first even i >= start */
        idx_t first = (start & 1) ? start + 1 : start;
        if (first > max_i)
            continue;  /* no even i in this range */

        /* last even i <= max_i */
        idx_t last = (max_i & 1) ? max_i - 1 : max_i;

        /* number of even terms */
        idx_t count = (last - first) / 2 + 1;

        /* sum of the arithmetic progression of evens */
        seq_t sum = (seq_t)count * ((seq_t)first + (seq_t)last) / 2;

        ans += sum;
    }
    return ans;
}
