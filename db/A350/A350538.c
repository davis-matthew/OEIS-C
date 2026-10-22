/*
 * A350538  a(n) is the smallest proper multiple of n which contains only even
 *          digits.
 * https://oeis.org/A350538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 98, u32 n <= 9998, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   all_even_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/all_even_digits.h"

/* Computes the smallest proper multiple of n that contains only even
   decimal digits (search). */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast to seq_t for safe arithmetic   */
    seq_t cand = nn * 2;          /* first proper multiple (k = 2)       */

    while (!all_even_digits(cand)) {
        cand += nn;               /* next multiple: (k+1)*n             */
    }
    return cand;
}
