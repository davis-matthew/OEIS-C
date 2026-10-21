/*
 * A261720  Array of pyramidal (3-dimensional figurate numbers) read by
 *          antidiagonals.
 * https://oeis.org/A261720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(n,k) = k*(k+1)*((k-1)*n + 3)/6, where (n,k) are the row
 *            and column indices of the position n in the array read by
 *            antidiagonals (rows and columns start at 1)
 * Range:     u8 n <= 52, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   antidiagonal_from_index, tri_prefix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/antidiagonal_from_index.h"
#include "../utils/tri_prefix.h"

/* Public function required by the build. */
seq_t a(idx_t n)
{
    idx_t row, col;
    antidiagonal_from_index(n, &row, &col);

    /* Compute T(row,col) = col*(col+1)*((col-1)*row + 3)/6 safely. */
    unsigned __int128 c  = col;
    unsigned __int128 r  = row;
    unsigned __int128 val =
        c * (c + 1) * ((c - 1) * r + 3) / 6;
    return (seq_t)val;
}
