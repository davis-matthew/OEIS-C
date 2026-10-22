/*
 * A354047  A169683 read as ternary numbers.
 * https://oeis.org/A354047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 2557, u32 n <= 9629, u64 n <= 9629,
 *            u128 n <= 9629, big any n
 * Helpers:   floor_log2_uint64, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0;
   for n ≥ 1, let k = ⌊log₂(n+1)⌋.
   Then a(n) = a(n - (2ᵏ - 1)) + 3^(k-1).
   The iterative implementation below uses the relation to
   compute the value in O(log n) time with only integer operations. */

#include <stdint.h>
#include "../utils/pow3.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    while (n != 0) {
        /* find k so that 2ᵏ ≤ n+1 < 2ᵏ⁺¹ */
        idx_t m = n + 1;
        unsigned k = floor_log2_uint64(m);   /* k ≥ 1 because n ≥ 1 */

        /* add the 3^(k-1) term */
        result += pow3((idx_t)(k - 1));

        /* reduce n to the index inside the previous block */
        n -= ((idx_t)1 << k) - 1;
    }
    return result;
}
