/*
 * A278481  Number of neighbors of the n-th term in a full isosceles triangle
 *          read by rows.
 * https://oeis.org/A278481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 2 if n = 1; otherwise let r = ceil((sqrt(8n+1)-1)/2)
   and k = n - r(r-1)/2.  Then a(n) = 4 if k = 1 or k = r (border),
   else a(n) = 6 (interior).  The triangle is read row‑by‑row,
   with first row length 1, second row length 2, …  */

#include <stdint.h>

/* compute floor(sqrt(x)) for a 64‑bit unsigned integer */
static uint64_t floor_sqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; 0 is unused but harmless */
        return (seq_t)0;
    if (n == 1)          /* apex of the triangle */
        return (seq_t)2;

    /* determine the row r containing the n‑th element */
    uint64_t m   = 8 * (uint64_t)n + 1;
    uint64_t s   = floor_sqrt_u64(m);
    idx_t r      = (s - 1) / 2;              /* floor of (sqrt(8n+1)-1)/2 */
    unsigned __int128 tri_r = (unsigned __int128)r * (r + 1) / 2;
    if ((idx_t)tri_r < n)
        r++;

    /* position inside the row (1‑based) */
    idx_t prev_T = (r - 1) * r / 2;
    idx_t k = n - prev_T;

    /* left or right border */
    if (k == 1 || k == r)
        return (seq_t)4;

    /* interior cell */
    return (seq_t)6;
}
