/*
 * A286903  {0->00}-transform of the Sturmian word A080764.
 * https://oeis.org/A286903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*r) - floor((n+2)*r) + 1, where r = (3 -
 *            sqrt(2))/7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_alpha
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_alpha.h"

seq_t a(idx_t n)
{
    /* a(n) = 1 if floor(n*r) == floor((n+2)*r), otherwise 0. */
    seq_t f1 = floor_alpha(n);
    seq_t f2 = floor_alpha(n + 2);
    return (f1 == f2) ? (seq_t)1 : (seq_t)0;
}
