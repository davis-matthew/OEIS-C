/*
 * A357177  Prime indices of the Heegner numbers (A003173).
 * https://oeis.org/A357177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = primepi(A003173(n)) where A003173 =
 *            {1,2,3,7,11,19,43,67,163}
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Heegner numbers (A003173) */
    static const idx_t heegner[9] = {1, 2, 3, 7, 11, 19, 43, 67, 163};

    /* Indexing starts at 1; if n is beyond the known terms, return 0 */
    if (n <= 9) {
        return compute_pi(heegner[n - 1]);
    }
    return (seq_t)0;
}
