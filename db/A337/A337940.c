/*
 * A337940  Triangle read by rows: T(n, k) = T(n+2) - T(n-k), with the
 *          triangular numbers T = A000217, for n >= 1, k = 1, 2, ..., n.
 * https://oeis.org/A337940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = n‑th entry of the triangle T(n,k) read row‑by‑row
    T(n,k) = ((k+2)*(2*n-k+3))/2,
    where for a given index n we first find the row r such that
      r*(r+1)/2  >=  n  > (r-1)*r/2
    and then k = n - (r-1)*r/2.  The computation uses 128‑bit
    arithmetic for the intermediate products, so the result
    is exact for all arguments that fit into the returned type.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but harmless */

    /* Find the row r: smallest integer with r*(r+1)/2 >= n. */
    idx_t r = 1;
    while ( (unsigned __int128)r * (r + 1) / 2 < n )
        ++r;

    /* Number of elements before this row. */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;

    /* Column index inside the row (1‑based). */
    idx_t k = n - prev;         /* 1 <= k <= r */

    /* Compute the triangle entry using 128‑bit intermediate.
       All intermediate arithmetic is performed in unsigned __int128
       to avoid overflow on 64‑bit indices and to keep the result
       accurate for small seq_t types as well. */
    unsigned __int128 sk = (unsigned __int128)k;
    unsigned __int128 sr = (unsigned __int128)r;

    unsigned __int128 product = (sk + 2) * (2 * sr - sk + 3);
    unsigned __int128 val = product / 2;

    return (seq_t)val;
}
