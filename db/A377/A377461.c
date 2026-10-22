/*
 * A377461  Number of ranked labeled trees compatible with the 2-leaf perfect
 *          phylogeny of sample size n that possesses the largest number of
 *          compatible ranked labeled trees.
 * https://oeis.org/A377461
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-2)! * ⌊n/2⌋! * ⌈n/2⌉! / 2^(n-2) (derived from the OEIS
 *            definition)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute floor(n/2) and ceil(n/2) */
    idx_t f = n / 2;           /* floor(n/2)  */
    idx_t c = (n + 1) / 2;     /* ceil(n/2)   */

    /* Number of twos that must be removed from the product */
    idx_t twos = n - 2;

    seq_t result = (seq_t)1;

    /* Multiply all factors of (n-2)! */
    for (idx_t i = 1; i <= n - 2; ++i) {
        idx_t x = i;
        while (twos > 0 && (x & 1) == 0) {
            x >>= 1;          /* divide by 2 */
            --twos;
        }
        result *= (seq_t)x;
    }

    /* Multiply all factors of f!   */
    for (idx_t i = 1; i <= f; ++i) {
        idx_t x = i;
        while (twos > 0 && (x & 1) == 0) {
            x >>= 1;
            --twos;
        }
        result *= (seq_t)x;
    }

    /* Multiply all factors of c!   */
    for (idx_t i = 1; i <= c; ++i) {
        idx_t x = i;
        while (twos > 0 && (x & 1) == 0) {
            x >>= 1;
            --twos;
        }
        result *= (seq_t)x;
    }

    /* Any remaining powers of two that were not cancelled
       are removed by a final division.  For integer seq_t
       this operation is cheap and supported. */
    if (twos > 0) {
        seq_t denom = pow2(twos);
        result /= denom;
    }

    return result;
}
