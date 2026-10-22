/*
 * A378635  Triangle T(n,k) read by rows, where row n is a permutation of
 *          numbers 1 through n, such that if the deck of n cards is prepared
 *          in this order, and under-down dealing is used, then the resulting
 *          cards are put down in increasing order.
 * https://oeis.org/A378635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (r,c) entry of the Josephus triangle T(n,k) *
 *            defined by T(1,1)=1, * T(n,1)=T(n-1,n-1)+1, * T(n,2)=1, *
 *            T(n,k)=T(n-1,k-2)+1 for n>1 and k>2. * The flattened triangle
 *            read by rows gives the sequence A378635
 * Range:     u8 n <= 9415, u16 n <= 9415, u32 n <= 9415, u64 n <= 9415,
 *            u128 n <= 9415, big any n
 * Helpers:   T_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/T_row.h"

/* Find smallest r such that r*(r+1)/2 >= n */
static inline idx_t tri_row(idx_t n) {
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        if (high > (idx_t)(-1) / 2) break;            /* safety for huge n */
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 v = (unsigned __int128)mid * (mid + 1) / 2;
        if (v >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* undefined for n<1, return 0 for safety */

    idx_t r = tri_row(n);
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev;   /* column within row r */

    return T_row(r, c);
}
