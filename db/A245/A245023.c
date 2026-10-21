/*
 * A245023  Number of cases of tie (no winner) in the n-person
 *          rock-paper-scissors game.
 * https://oeis.org/A245023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 3*(2^n-2) (n >= 1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* for safety, though offset starts at 1 */
    seq_t p3 = pow3(n);            /* compute 3^n   */
    seq_t p2 = pow2(n);            /* compute 2^n   */
    seq_t res = p3 - (p2 * 3) + 6; /* 3^n - 3*2^n + 6 */
    return res;
}
