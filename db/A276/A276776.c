/*
 * A276776  Volume of spherical shell between two spheres of radius n and k,
 *          rounded to the nearest integer. Triangular array T(n, k), read by
 *          rows.
 * https://oeis.org/A276776
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = round((4/3)*π*(R³ – K³)) where R and K are obtained from
 *            the linear index n by mapping to the triangular array T(R,K): R
 *            ≥ 2, 1 ≤ K < R. The offset is 2, i.e. a(2) is the first entry
 * Range:     u8 none, u16 none, u32 none, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row R of the triangular array that contains the index n.
   The linear position is p = n−1 (p ≥ 1).  R is the smallest integer
   satisfying R·(R–1)/2 ≥ p. */
static inline idx_t triangular_row(idx_t p)
{
    idx_t low = 2, high = 2;
    /* Exponential search to find an upper bound */
    while (( (__int128)(high - 1) * high ) / 2 < p) high <<= 1;
    /* Binary search within the bound */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (( (__int128)(mid - 1) * mid ) / 2 >= p)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for indices below the offset */

    /* Linear position in the triangle */
    idx_t p = n - 1;

    /* Outer radius R */
    idx_t R = triangular_row(p);

    /* Elements before this row */
    idx_t prev = (R - 2) * (R - 1) / 2;
    /* Position within the row (1‑based) */
    idx_t pos = p - prev;
    /* Inner radius K */
    idx_t K = pos;

    /* Compute R³ and K³ – these are < 2⁶⁴ for typical bounds (R ≤ 10⁴) */
    seq_t R3 = (seq_t)R * (seq_t)R * (seq_t)R;
    seq_t K3 = (seq_t)K * (seq_t)K * (seq_t)K;
    seq_t diff = R3 - K3;          /* R³ – K³ */

    /* High‑precision rational approximation of (4/3)·π.
       π ≈ 3.1415926535897932384626433832795…  */
    const seq_t PI_NUM = 3141592653590ULL;        /* π · 10¹² (rounded) */
    const seq_t PI_DEN = 1000000000000ULL;        /* 10¹²            */
    const seq_t NUM    = 4 * PI_NUM;              /* 4·π·10¹²        */
    const seq_t DEN    = 3 * PI_DEN;              /* 3·10¹²          */

    /* Perform (diff · NUM) / DEN with rounding to nearest integer */
    __int128 prod = (__int128)diff * (__int128)NUM;   /* up to ≈ 1.26·10²⁵ */
    __int128 div  = (__int128)DEN;                   /* 3·10¹²           */
    seq_t result = (seq_t)((prod + div / 2) / div);   /* round half up     */

    return result;
}
