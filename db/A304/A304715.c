/*
 * A304715  For any n > 0, if A006666(n) >= 0, then a(n) = Sum_{i =
 *          0..A006666(n)-1} 2^i * [T^i(n) == 0 (mod 2)] (where [] is an
 *          Iverson bracket and T^i denotes the i-th iterate of the Collatz
 *          function A014682); otherwise a(n) = -1.
 * https://oeis.org/A304715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Sum_{i = 0..A006666(n)-1} 2^i * [T^i(n) == 0 (mod 2)]
 *            Recurrence used for the implementation: a(1) = 0 a(2*n) = 2*a(n)
 *            + 1 a(odd n > 1) = 2*a((3*n + 1)/2)
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 2462,
 *            big any n
 * Helpers:   collatz_a_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard header for uint64_t type */
#include <stdint.h>
#include "../utils/collatz_a_rec.h"

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n <= 0, return -1 */
        return (seq_t)-1;
    return collatz_a_rec(n);
}
