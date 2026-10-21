/*
 * A288677  Every element of Z/nZ can be expressed as a sum of no more than
 *          a(n) squares.
 * https://oeis.org/A288677
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n=2; 2 if n≠0 mod 4 and every prime squared divisor
 *            p satisfies p≡1 mod 4; * otherwise 3 if n≠0 mod 8, else 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   squarefree_condition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/squarefree_condition.h"

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)1;

    if (n % 4 != 0) {
        if (squarefree_condition(n))
            return (seq_t)2;
    }

    return (n % 8 != 0) ? (seq_t)3 : (seq_t)4;
}
