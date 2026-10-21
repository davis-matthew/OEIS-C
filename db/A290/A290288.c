/*
 * A290288  Sum of the differences of the larger and smaller parts in the
 *          partitions of 2n into two parts with the larger part prime.
 * https://oeis.org/A290288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Σ_{i=1..n} (n - i) * A010051(2*n - i) Where
 *            A010051(k) = 1 if k is prime, 0 otherwise
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1; below the offset just returns 0 */
    if (n < 1)
        return 0;

    seq_t total = 0;
    /* loop over p = 2n - i, i = 1..n  →  p ranges from n+1 to 2n-1 */
    idx_t start = n + 1;
    idx_t end   = n + (n - 1);          /* 2*n - 1, no risk of overflow for n ≥ 1 */

    for (idx_t p = start; p <= end; ++p)
    {
        if (is_prime(p))
        {
            /* contribution for this prime p is 2 * (p - n) */
            seq_t diff = (seq_t)(p - n);     /* p - n ≥ 1 */
            total += diff << 1;              /* multiply by 2 */
        }
    }

    return total;
}
