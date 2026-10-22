/*
 * A379966  a(n) = 1 if n is an even squarefree number, otherwise 0.
 * https://oeis.org/A379966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is an even squarefree number, otherwise 0
 * Range:     u8 n <= 6561, u16 n <= 9573, u32 n <= 9573, u64 n <= 9573,
 *            u128 n <= 9573, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0 && is_squarefree((seq_t)n))
        return (seq_t)1;
    return (seq_t)0;
}
