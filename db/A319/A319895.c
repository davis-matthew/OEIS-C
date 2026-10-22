/*
 * A319895  a(n) is the number of partitions of n into consecutive parts, plus
 *          the total number of parts in those partitions.
 * https://oeis.org/A319895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001227(n) + A204217(n) * A001227(n): number of
 *            partitions of n into consecutive parts * A204217(n): total
 *            number of parts in all those partitions * This implementation
 *            enumerates over the possible length k of the * consecutive-part
 *            partition. For each k such that * 1 ≤ k and k(k+1)/2 ≤ n...
 * Range:     u8 n <= 137, u16 n <= 8797, u32 n <= 8797, u64 n <= 8797,
 *            u128 n <= 8797, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */

    seq_t count = 0;
    seq_t sumlen = 0;

    /* iterate over possible lengths k, 1 ≤ k, k(k+1)/2 ≤ n */
    for (idx_t k = 1; ; ++k)
    {
        seq_t tri = (seq_t)k * (k + 1) / 2;
        if (tri > n) break;

        seq_t rem = (seq_t)n - (seq_t)k * (k - 1) / 2;
        if (rem % k == 0)
        {
            ++count;
            sumlen += k;
        }
    }

    return count + sumlen;
}
