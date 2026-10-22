/*
 * A337638  Consider the Conant Gasket G_{2n}, as in A328080; a(n) = area of
 *          largest cell when G_{2n} is drawn on a scale at which the smallest
 *          cell has area 1.
 * https://oeis.org/A337638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n! / 2^(max(n-3, 0)) for n ≥ 3, with a(0) = 1, a(1) = 2,
 *            a(2) = 3. This follows from the definition of the largest cell
 *            in the Conant Gasket G_{2n} when the smallest cell has area 1
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    /* For n ≥ 3 compute n! / 2^(n-3). */
    idx_t k;
    seq_t result = (seq_t)1;
    idx_t rem = n - 3;        /* how many factors of 2 to cancel */

    for (k = (idx_t)1; k <= n; ++k)
    {
        result *= (seq_t)k;

        /* Cancel factors of 2 as we go to keep the number small. */
        while (rem > 0 && (result & (seq_t)1) == (seq_t)0)
        {
            result /= (seq_t)2;
            --rem;
        }
    }

    /* If any required cancellation remains (unlikely), perform it */
    while (rem > 0)
    {
        result /= (seq_t)2;
        --rem;
    }

    return result;
}
