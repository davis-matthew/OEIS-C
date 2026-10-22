/*
 * A367844  Triangle read by rows: T(n, k) = (n+5)*n/2 + 1 + (n^2 mod 3) - 3*k
 *          for 0 <= k <= n.
 * https://oeis.org/A367844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (r+5)*r/2 + 1 + (r^2 mod 3) – 3*k * where r =
 *            floor((sqrt(1+8*n)-1)/2) and k = n – r*(r+1)/2 * (r is the row
 *            index, k the column index in that row of the triangle
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* --- Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 --- */
    idx_t low = 0, high = n;   /* r cannot exceed n */
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        /* Use unsigned __int128 to avoid overflow */
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val == n) {
            r = mid;
            break;
        } else if (val < n) {
            low = mid + 1;
            r = mid;   /* latest mid that satisfies the inequality */
        } else {
            high = mid - 1;
        }
    }
    /* Compute k = n - r*(r+1)/2; r*(r+1)/2 fits in idx_t after cast */
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;   /* start index of row r */
    idx_t k = n - start;

    /* Value of T(r,k) */
    seq_t rr = (seq_t)r;
    seq_t base = ((rr + 5) * rr) / 2 + 1;
    seq_t rem = ((rr % 3) * (rr % 3)) % 3;   /* r^2 mod 3 */
    base += rem;
    seq_t term = base - 3 * (seq_t)k;
    return term;
}
