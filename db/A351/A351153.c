/*
 * A351153  Triangle read by rows: T(n, k) = n*(k - 1) - k*(k - 3)/2 with 0 <
 *          k <= n.
 * https://oeis.org/A351153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 1 + (k-1)*(n+1) - (k-1)*k/2, where k = n - r*(r-1)/2 and
 *            r = ceil((sqrt(1+8n)-1)/2)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the triangle T(n,k)=n*(k-1)-k*(k-3)/2 read by rows. */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer: floor(sqrt(x)). */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1ULL << 32);          /* high^2 fits in 128 bits   */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n<1, but harmless */

    /* compute r = ceil((sqrt(1+8n)-1)/2) */
    idx_t k = (idx_t)((__int128)8 * n + 1);   /* 1 + 8*n */
    idx_t s = floor_sqrt_u64(k);
    idx_t r = (s - 1) / 2;
    /* ensure r*(r+1)/2 >= n */
    while ( ( (__int128)r * (r + 1) ) / 2 < n ) r++;

    idx_t prev_tri = (idx_t)( ( (__int128)r * (r - 1) ) / 2 );
    idx_t c = n - prev_tri;                  /* 1 <= c <= r */

    /* T(r,c) = 1 + (c-1)*(r+1) - (c-1)*c/2 */
    seq_t sr = (seq_t)r;
    seq_t sc = (seq_t)c;
    seq_t term1 = (seq_t)(sc - 1) * (sr + 1);
    seq_t term2 = (seq_t)(sc - 1) * sc / 2;
    return 1 + term1 - term2;
}
