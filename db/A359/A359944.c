/*
 * A359944  Number of divisors d of n such that d-1 is a cube.
 * https://oeis.org/A359944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ m ≥ 0 : (m³ + 1) divides n }
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t m = 0; ; ++m)
    {
        idx_t cube = m * m * m;          /* m³ */
        if (cube + 1 > n)
            break;
        if (n % (cube + 1) == 0)
            ++count;
    }
    return count;
}
