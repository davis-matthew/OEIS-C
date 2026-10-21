/*
 * A249945  a(n) = n! + 3^n.
 * https://oeis.org/A249945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + 3^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   factorial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t f = factorial(n);
    seq_t p = pow3(n);
    return f + p;
}
