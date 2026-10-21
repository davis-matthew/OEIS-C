/*
 * A283683  Unique sequence with a(1)=0, a(2)=1, representing an array T(i,j)
 *          read by antidiagonals in which every row is this sequence itself.
 * https://oeis.org/A283683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   not stated
 * Range:     u8 n <= 9379, u16 n <= 9379, u32 n <= 9379, u64 n <= 9379,
 *            u128 n <= 9379, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A283683
   Formula: a(n) = A020906(n) – 1
   The sequence is defined by a(1)=0, a(2)=1 and for n>2
   a(n) = a( j ) where
       d  = ceil( (√(8·n+1) – 1) / 2 )          // diagonal index
       t  = d·(d–1)/2          (number of elements in previous diagonals)
       p  = n – t              (position within the current diagonal, 1‑based)
       j  = d – p + 1          (column index in the table, 1‑based)
   The algorithm repeatedly applies this reduction until the index
   is 1 or 2, then returns 0 or 1 accordingly. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)            // a(1) = 0
        return (seq_t)0;
    if (n == 2)            // a(2) = 1
        return (seq_t)1;

    while (n > 2) {
        /* Find the smallest integer d with d*(d+1)/2 ≥ n */
        idx_t low  = 1, high = 1;
        while ((unsigned __int128)high * (high + 1) / 2 < n)
            high <<= 1;           // exponentially increase upper bound

        while (low < high) {
            idx_t mid = (low + high) >> 1;
            if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
                high = mid;        // mid is big enough, search lower half
            else
                low = mid + 1;     // mid too small, search upper half
        }
        idx_t d = high;                 // diagonal index

        idx_t tprev = d * (d - 1) / 2;   // elements before this diagonal
        idx_t p = n - tprev;            // 1‑based position within the diagonal
        n = d - p + 1;                  // new index for the next iteration
    }

    /* After the loop n ∈ {1,2} */
    return (n == 1) ? (seq_t)0 : (seq_t)1;
}
