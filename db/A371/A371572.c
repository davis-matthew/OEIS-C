/*
 * A371572  Irregular triangle read by rows: row n lists the 1-based positions
 *          (starting from the left) of ones in the binary expansion of n,
 *          with row 0 = 0.
 * https://oeis.org/A371572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 9808, u16 n <= 9808, u32 n <= 9808, u64 n <= 9808,
 *            u128 n <= 9808, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Compute A371572: irregular triangle read by rows.  
   For each n ≥ 0, let r be the row index such that the cumulative count
   of elements up to row r covers n.  Row 0 contains the single value 0.
   For r > 0, row r lists the 1‑based positions (left‑to‑right) of the
   1‑bits in the binary representation of r.  This function returns the
   n‑th term in the concatenated rows.
*/
#include <stdint.h>
#include <stdbool.h>

/* helper for popcount of 64‑bit values */
static inline unsigned int popcount_u64(uint64_t x)
{
    return __builtin_popcountll(x);
}

/* helper for number of bits needed to represent x (x > 0) */
static inline unsigned int bitlen_u64(uint64_t x)
{
    return x == 0 ? 1 : 64 - __builtin_clzll(x);
}

seq_t a(idx_t n)
{
    /* Row 0 is special: a(0) = 0 */
    if (n == 0)
        return (seq_t)0;

    /* Find the row r such that cumulative length up to row r-1 ≤ n < cumulative up to r */
    idx_t cum = 0;            /* cumulative count before current row */
    idx_t r = 0;              /* current row index */

    for (;; ++r)
    {
        idx_t len;
        if (r == 0)
            len = 1;                     /* row 0 has one element (0) */
        else
            len = (idx_t)popcount_u64((uint64_t)r);

        if (cum + len > n)
            break;                       /* n lies in this row */

        cum += len;                      /* advance past this row */
    }

    /* Position of the element within row r (0‑based) */
    idx_t posIndex = n - cum;

    /* Special case for row 0 (already handled above) -> unreachable here */

    /* Determine the posIndex‑th set bit from left in r */
    unsigned int bitlen = bitlen_u64((uint64_t)r);
    unsigned int ones_seen = 0;

    /* Scan bits from most significant to least significant */
    for (unsigned int bit = bitlen; bit > 0; --bit)
    {
        uint64_t mask = (uint64_t)1 << (bit - 1);
        if ((uint64_t)r & mask)
        {
            ++ones_seen;
            if (ones_seen == posIndex + 1)
            {
                /* position counting starts at 1 from the leftmost bit */
                return (seq_t)(bitlen - bit + 1);
            }
        }
    }

    /* Fall back (should never happen) */
    return (seq_t)0;
}
