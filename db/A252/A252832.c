/*
 * A252832  Number of n X 4 nonnegative integer arrays with upper left 0 and
 *          every value within 2 of its king move distance from the upper left
 *          and every value increasing by 0 or 1 with every step right,
 *          diagonally se or down.
 * https://oeis.org/A252832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 480*n^2 - 3394*n + 6449 for n > 5 a(1)=7, a(2)=29,
 *            a(3)=127, a(4)=493, a(5)=1474 (All values are computed in type
 *            seq_t, an unsigned integer type.)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_values[6] = {0, 7, 29, 127, 493, 1474};

seq_t a(idx_t n)
{
    if (n <= 5)
        return init_values[(size_t)n];

    /* Compute 480*n^2 - 3394*n + 6449 */
    seq_t nn = (seq_t)n;
    seq_t result = 480 * nn * nn;
    result -= 3394 * nn;
    result += 6449;
    return result;
}
