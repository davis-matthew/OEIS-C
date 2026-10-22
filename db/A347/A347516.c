/*
 * A347516  Number of divisors of n that are at most n^(1/3).
 * https://oeis.org/A347516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are at most n^(1/3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_int_cube_root.h"

/* Count divisors ≤ n^(1/3) */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* offset handling – a(1) is the first term */
    }
    idx_t limit = _int_cube_root(n);
    seq_t count = 0;
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            ++count;
        }
    }
    return count;
}
