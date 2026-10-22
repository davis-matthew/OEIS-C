/*
 * A343501  Positions of 4's in A003324.
 * https://oeis.org/A343501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 9587, u32 n <= 9587, u64 n <= 9587,
 *            u128 n <= 9587, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A343501
   Positions of 4's in A003324.
   Numbers of the form (2*k+1)*2^e with e>=1 and k+e even,
   i.e. union of {(4*m+1)*2^(2t)} and {(4*m+3)*2^(2t-1)} (t>0, m>=0).
   The nth term a(n) is the nth smallest such integer.
   This implementation uses a binary search on the value that
   satisfies count(x) >= n, where count(x) counts how many qualifying
   numbers are <= x.  The counting is done directly from the formula
   without constructing a sieve.
*/

#include <stdint.h>

static inline idx_t count_leq(idx_t x)
{
    /* Count numbers <= x of the two types:
       Type A: (4m+1)*2^(2t)  (e even, e>=2)
       Type B: (4m+3)*2^(2t-1) (e odd,  e>=1)
    */
    idx_t cnt = 0;
    for (int e = 1; ; ++e) {
        idx_t M = x >> e;          /* odd part upper bound */
        if (!M) break;             /* no further numbers */
        if ((e & 1) == 0) {        /* e even */
            cnt += (M + 3) / 4;
        } else {                   /* e odd */
            cnt += (M + 1) / 4;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but guard */
    /* Upper bound roughly 4*n (density ~1/4). Add cushion. */
    idx_t high = 4 * n + 100;
    idx_t low  = 1;

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (count_leq(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
