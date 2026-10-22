/*
 * A303660  Number of ways to write 2*n+1 as p + 3^k + 5^m, where p is a
 *          prime, and k and m are nonnegative integers.
 * https://oeis.org/A303660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of 2*n+1 as p + 3^k + 5^m,
 *            where p is prime, k and m are non‑negative integers
 * Range:     u8 n <= 498, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* offset starts at 1 */

    /* target odd number 2*n+1 */
    seq_t target = ((seq_t)n) * 2 + 1;

    seq_t count = 0;

    seq_t pow3 = 1;
    /* iterate over all k such that 3^k <= target‑2 */
    while (pow3 <= target - 2u) {
        /* maximum value that 5^m can take for this k */
        seq_t bound = target - pow3 - 2u;

        seq_t pow5 = 1;
        /* iterate over all m such that 5^m <= bound */
        while (pow5 <= bound) {
            seq_t p = target - pow3 - pow5;
            if (p >= 2u && is_prime_big(p))
                ++count;

            /* advance to the next power of 5, avoid overflow */
            if (pow5 > bound / 5u) break;
            pow5 *= 5u;
        }

        /* advance to the next power of 3, avoid overflow */
        if (pow3 > (target - 2u) / 3u) break;
        pow3 *= 3u;
    }

    return count;
}
