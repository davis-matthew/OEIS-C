/*
 * A399262  Triangle read by rows: row n gives the n consecutive integers
 *          centered at n^2, excluding n^2 if n is even.
 * https://oeis.org/A399262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) is the n-th element of the triangle read by rows. Find r
 *            such that r(r-1)/2 < n ≤ r(r+1)/2. Let pos = n - r(r-1)/2 - 1
 *            (0‑based position inside the row). Let m = r/2. If r is odd:
 *            a(n) = r^2 - m + pos. If r is even: if pos < m : a(n) = r^2 - m
 *            + pos; else : a(n) = r^2 + (pos - m + 1);
 * Range:     u8 n <= 128, u16 n <= 9295, u32 n <= 9295, u64 n <= 9295,
 *            u128 n <= 9295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; for safety we return 0 for n==0 */
        return (seq_t)0;

    /* Find the row r by binary search */
    idx_t low  = 1;
    idx_t high = 1;
    /* Expand high until C(high) >= n.  Use __int128 to avoid overflow. */
    while ( ((unsigned __int128)high * (high + 1) / 2) < n ) {
        if (high > (UINT64_MAX >> 1)) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 c = (unsigned __int128)mid * (mid + 1) / 2;
        if (c >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;  /* row number */

    /* Previous cumulative count */
    unsigned __int128 prev_big = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev_big;

    idx_t pos = n - prev - 1;   /* 0‑based position in the row */

    seq_t n2 = (seq_t)r * (seq_t)r;   /* r^2 */
    idx_t m = r / 2;                  /* floor(r/2) */

    seq_t result;
    if (r & 1U) { /* r is odd */
        result = n2 - (seq_t)m + (seq_t)pos;
    } else {      /* r is even */
        if (pos < m) {
            result = n2 - (seq_t)m + (seq_t)pos;
        } else {
            result = n2 + (seq_t)(pos - m + 1);
        }
    }
    return result;
}
