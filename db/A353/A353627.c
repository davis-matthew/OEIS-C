/*
 * A353627  a(n) = 1 if the odd part of n is squarefree, otherwise 0.
 * https://oeis.org/A353627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if odd part of n is squarefree, otherwise 0
 * Range:     u8 n <= 1178, u16 n <= 9949, u32 n <= 9949, u64 n <= 9949,
 *            u128 n <= 9949, big any n
 * Helpers:   is_squarefree, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A353627: a(n) = [odd_part(n) is squarefree]        */

#include "../utils/odd_part.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Obtain the odd part of n */
    seq_t odd = odd_part((seq_t)n);

    /* Use the provided helper to test squarefreeness */
    return (seq_t)(is_squarefree(odd) ? 1 : 0);
}
