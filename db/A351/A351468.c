/*
 * A351468  Irregular triangle read by rows where row n is Newey's sequence
 *          containing all permutations of 1..n.
 * https://oeis.org/A351468
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1057, u16 n <= 8643, u32 n <= 8643, u64 n <= 8643,
 *            u128 n <= 8643, big any n
 * Helpers:   cum_len, newey_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Newey’s irregular triangle:  
 *  Each row n ≥ 2 has n²‑2n+4 terms.  
 *  a(n) returns the term at overall position n (offset 2).  */

#include <stdint.h>
#include "../utils/cum_len.h"
#include "../utils/newey_T.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* undefined for n < 2 */

    idx_t pos = n - 2;                  /* 0‑based index in the flattened triangle */

    /* Find the row r such that cum_len(r-1) ≤ pos < cum_len(r) */
    idx_t low  = 2;
    idx_t high = 2;
    while (cum_len(high) <= (seq_t)pos)
        high *= 2;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum_len(mid) > (seq_t)pos)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    seq_t prevCum = (r > 2) ? cum_len(r - 1) : 0;
    idx_t col = (idx_t)(((seq_t)pos - prevCum) + 1);   /* 1‑based column within the row */

    return newey_T(r, col);
}
