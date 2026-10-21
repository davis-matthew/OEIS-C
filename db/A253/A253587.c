/*
 * A253587  The sum of the i-th ternary digits of n, k, and T(n,k) equals 0
 *          (mod 3) for each i>=0 (leading zeros included); triangle T(n,k),
 *          n>=0, 0<=k<=n, read by rows.
 * https://oeis.org/A253587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 9829, u16 n <= 9829, u32 n <= 9829, u64 n <= 9829,
 *            u128 n <= 9829, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   Triangle T is defined by  
       T(0,0) = 0  
       T(n,k) = 3 * T( floor(n/3), floor(k/3) ) + ( (6 - n%3 - k%3) % 3 )  
   Sequence a(n) is the n‑th element of T when the triangle is read
   by rows (row 0 first, then row 1, etc.).  
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2   */
    idx_t low = 0, high = n;                     /* r <= n */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Column index within the row */
    idx_t count_before = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t col = n - count_before;

    /* Compute T(r, col) using the digitwise formula below.   */
    seq_t result = 0;
    seq_t power  = 1;   /* 3^i as a seq_t */

    while (r > 0 || col > 0) {
        unsigned int r_digit = r % 3;
        unsigned int c_digit = col % 3;
        unsigned int sum_mod = (r_digit + c_digit) % 3;
        unsigned int delta = (3 - sum_mod) % 3;        /* (-r_digit - c_digit) mod 3 */
        result += (seq_t)delta * power;
        power  *= 3;
        r    /= 3;
        col  /= 3;
    }

    return result;
}
