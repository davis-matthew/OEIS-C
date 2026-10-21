/*
 * A284447  Permutation of the positive integers: a(n) = A258996(A092569(n)) =
 *          A092569(A258996(n)).
 * https://oeis.org/A284447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n XOR f(n) where f(n) = sum of 2^(2k) for each odd power
 *            2^(2k+1) that is ≤ n (i.e., f(n) = 4 for 8…31, 20 for 32…63, 84
 *            for 128…511, …) This yields a self‑inverse permutation with
 *            fixed points 1..7
 * Range:     u8 n <= 255, u16 n <= 9868, u32 n <= 9868, u64 n <= 9868,
 *            u128 n <= 9868, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 7)
        return (seq_t)n;

    /* Compute f(n): sum of 2^(2k) for odd exponents 2k+1 ≤ n.
       Start with the first odd power 2^3=8. */
    uint64_t offset = 0;
    for (uint64_t t = 8; t <= n; ) {
        offset += t >> 1;          /* add 2^(2k) = t/2            */
        /* Avoid overflow on the next shift */
        if (t > n >> 2)
            break;
        t <<= 2;                   /* next odd power 2^(2k+3)     */
    }

    return (seq_t)((seq_t)n ^ (seq_t)offset);
}
