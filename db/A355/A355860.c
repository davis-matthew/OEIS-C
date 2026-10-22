/*
 * A355860  Triangle read by rows: T(n,k) = n*k/(n + k) if n+k divides n*k,
 *          otherwise T(n,k) = 0; n >= 1, k >= 1.
 * https://oeis.org/A355860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by mapping n into the triangle T with rows
 *            indexed by r and columns by k: T(r,k)=r*k/(r+k) if divisible,
 *            otherwise 0. The index n is converted to row r and column k by
 *            solving r(r+1)/2 ≥ n
 * Range:     u8 n <= 9232, u16 n <= 9232, u32 n <= 9232, u64 n <= 9232,
 *            u128 n <= 9232, big any n
 * Helpers:   sqrt128_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sqrt128_floor.h"

/* Compute a(n) for the OEIS sequence A355860 */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n */
    uint64_t r = 0;
    {
        unsigned __int128 val = (unsigned __int128)8 * n + 1;
        uint64_t sq = sqrt128_floor(val);
        r = (sq - 1) >> 1;    /* (sq - 1)/2 */

        /* Adjust in case of rounding errors */
        while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;
        while (r > 0 && (unsigned __int128)(r - 1) * r / 2 >= n) --r;
    }

    /* Column k within the row */
    uint64_t prev = (unsigned __int128)(r - 1) * r / 2;   /* (r-1)r/2 */
    uint64_t k = n - prev;                               /* 1 ≤ k ≤ r */

    /* Compute T(r,k) if divisible, else 0 */
    unsigned __int128 prod = (unsigned __int128)r * k;
    unsigned __int128 denom = (unsigned __int128)(r + k);
    if (prod % denom == 0)
        return (seq_t)(prod / denom);
    else
        return (seq_t)0;
}
