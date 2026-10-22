/*
 * A375616  a(n) is the number of lucky cars in all parking functions of order
 *          n.
 * https://oeis.org/A375616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^(n-2) * n * (n+3) / 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    /* compute (n+1)^(n-2) */
    seq_t base = pow_seq((seq_t)(n + 1), (int)(n - 2));

    /* result = base * n * (n+3) / 2 */
    seq_t result = base * (seq_t)n;
    result = result * (seq_t)(n + 3);
    result = result / 2;

    return result;
}
