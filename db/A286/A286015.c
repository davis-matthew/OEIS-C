/*
 * A286015  Sum of largest parts of all partitions of n into consecutive
 *          parts.
 * https://oeis.org/A286015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the largest parts in all partitions of n into
 *            consecutive parts
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the value of the sequence for the given n (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset starts at 1; a(0) is unused */
        return 0;

    seq_t sum = 0;     /* result to return */

    /* Enumerate all possible numbers of parts k.  A partition exists iff
       k(k+1)/2 ≤ n and (n - k(k-1)/2) is divisible by k.          */
    for (idx_t k = 1; k * (k + 1) / 2 <= n; ++k) {
        idx_t numerator = n - k * (k - 1) / 2;    /* = k * first_part */
        if (numerator % k != 0)         /* not an integer first part */
            continue;
        idx_t first_part = numerator / k;
        if (first_part == 0)            /* parts must be positive */
            continue;
        idx_t largest_part = first_part + k - 1;
        sum += (seq_t)largest_part;
    }

    return sum;
}
