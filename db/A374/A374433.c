/*
 * A374433  Triangle read by rows: T(n, k) = Product_{p in PF(n) intersect
 *          PF(k)} p, where PF(a) is the set of the prime factors of a.
 * https://oeis.org/A374433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = rad(gcd(r, k))
 * where r and k are the row and column of n in triangular 
 * indexing: n = r(r+1)/2 + k, 0 ≤ k ≤ r.
 * For k = 0 (first element of each row) T(r,0) = 1.
 */

#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/rad.h"   /* seq_t rad(idx_t m);      */

seq_t a(idx_t n) {
    /* find largest r with r(r+1)/2 <= n  (binary search) */
    idx_t low = 0;
    idx_t high = n;                 /* r never exceeds n           */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    idx_t r = low;

    /* compute k = n - r(r+1)/2   (use 128 bits for the product) */
    unsigned __int128 tri_r = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri_r;

    /* first element of a row is 1 */
    if (k == 0) return (seq_t)1;

    /* compute rad(gcd(r,k)) */
    seq_t g = gcd((seq_t)r, (seq_t)k);
    return rad((idx_t)g);
}
