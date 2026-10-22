/*
 * A377660  Triangle read by rows: T(n, k) = (n - k)!*(n + k)!/n!.
 * https://oeis.org/A377660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 53, u64 n <= 134,
 *            u128 n <= 349, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A377660: Triangle read by rows.  
   T(n, k) = (n - k)! * (n + k)! / n!
   The sequence a(n) is the triangle flattened row‑wise, with a(0) = T(0,0).
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* find the row r such that n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }

    /* column c within the row */
    idx_t c = n - r * (r + 1) / 2;

    /* compute T(r, c) = (r - c)! * product_{i=1}^{c} (r + i) */
    seq_t result = 1;

    /* (r - c)! */
    for (idx_t t = 1; t <= r - c; ++t) {
        result *= (seq_t)t;
    }

    /* product (r+1)*(r+2)*…*(r+c) */
    for (idx_t t = 1; t <= c; ++t) {
        result *= (seq_t)(r + t);
    }

    return result;
}
