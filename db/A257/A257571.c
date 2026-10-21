/*
 * A257571  Triangular array read by rows: d(h,k) = distance between h and k
 *          in the tree at A232558, for h >=0, k = 0..h.
 * https://oeis.org/A257571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   depth_of, parent_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/depth_of.h"
#include "../utils/parent_of.h"
/* 
 * Formula: a(n) = distance between nodes h and k in the tree where
 * for each x > 0 the unique parent is
 *     parent(x) = (x % 2 == 0) ? x/2 : x-1 .
 * The triangular array is read row by row: for 0 ≤ k ≤ h,
 * a(n) = d(h, k) with n = 1 + h(h+1)/2 + k.
 */
 

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset 1; values for n < 1 are undefined */
        return (seq_t)0;

    /* Convert the linear index n to triangular coordinates h, k. */
    idx_t m = n - 1;          /* zero‑based index */
    idx_t h = 0;              /* current row */
    idx_t cum = 0;            /* number of elements before row h */

    while (1)
    {
        idx_t row_len = h + 1;
        if (m - cum < row_len)
            break;            /* n lies in row h */
        cum += row_len;
        ++h;
    }

    idx_t k = m - cum;        /* column index within row h */

    /* Compute the distance between h and k using parent links. */
    idx_t steps = 0;
    idx_t dh = depth_of(h);
    idx_t dk = depth_of(k);

    while (h != k)
    {
        if (dh > dk)
        {
            h = parent_of(h);
            --dh;
            ++steps;
        }
        else if (dk > dh)
        {
            k = parent_of(k);
            --dk;
            ++steps;
        }
        else                /* depths equal and nodes different */
        {
            h = parent_of(h);
            k = parent_of(k);
            --dh;
            --dk;
            steps += 2;
        }
    }

    return (seq_t)steps;
}
