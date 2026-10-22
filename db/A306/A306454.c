/*
 * A306454  a(n) = A261327(n)/A013946(n).
 * https://oeis.org/A306454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A261327(n)/A013946(n) Equivalent to a(n) = A008833(n^2+4)
 *            if n is odd = A008833((n^2+4)/4) if n is even where A008833(m)
 *            is the largest odd square dividing m. The function returns a(n)
 *            as described
 * Range:     u8 n <= 75, u16 n <= 1363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_odd_square_odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_odd_square_odd_part.h"

seq_t a(idx_t n)
{
    /* Compute k = n^2 + 4   (odd n)
             k = (n^2 + 4)/4  (even n)
       Use 128‑bit arithmetic to avoid overflow for large n.
    */
    unsigned __int128 temp;
    uint64_t k;

    if ((n & 1ULL) != 0) {            /* odd n */
        temp = (unsigned __int128)n * n + 4;
        k = (uint64_t)temp;           /* fits in 64 bits for the test range */
    } else {                          /* even n */
        temp = ((unsigned __int128)n * n + 4) / 4;
        k = (uint64_t)temp;
    }

    return largest_odd_square_odd_part(k);
}
