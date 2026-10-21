/*
 * A243223  Number of partitions of n into positive summands in arithmetic
 *          progression with common difference 3.
 * https://oeis.org/A243223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts arithmetic progressions with common difference 3,
 *            having at least two terms, that sum to n: n = (k)(2a + 3(k-1))/2
 *            with a >= 1, k >= 2
 * Range:     u8 n <= 148, u16 n <= 9817, u32 n <= 9817, u64 n <= 9817,
 *            u128 n <= 9817, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic with large values. */
    seq_t N = (seq_t)n;
    seq_t count = 0;
    seq_t twoN = N * 2;

    /* k is number of terms in the progression (>=2). */
    for (idx_t k = 2; ; ++k)
    {
        /* Stop when the minimal possible sum exceeds N. */
        seq_t min_sum = ((seq_t)k * ((seq_t)3 * k - 1)) / 2;
        if (min_sum > N)
            break;

        /* 2N must be divisible by k. */
        if (twoN % k != 0)
            continue;

        seq_t x = twoN / k;                     /* x = 2a + 3k - 3 */
        seq_t numerator = x - (seq_t)3 * k + 3;  /* = 2a */
        if (numerator <= 0)
            continue; /* a would be <= 0 */

        if (numerator & 1)            /* numerator must be even */
            continue;

        /* a = numerator / 2 is automatically >= 1 due to numerator>0 */
        (void)numerator; /* unused variable after check */
        count++;        /* valid progression found */
    }

    return count;
}
