/*
 * A373917  Triangle read by rows: T(n,k) = k*10 mod n, with n >= 1, k >= 0.
 * https://oeis.org/A373917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((n - (r−1)*r/2 − 1) * 10) mod r, where r is the smallest
 *            integer with r(r+1)/2 ≥ n (the row index of the triangle
 *            T(n,k)=k*10 mod n when read row‑wise)
 * Range:     u8 n <= 44, u16 n <= 5226, u32 n <= 9194, u64 n <= 9194,
 *            u128 n <= 9194, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row r such that r*(r+1)/2 ≥ n */
static idx_t
find_row(idx_t n)
{
    idx_t low  = 1;
    idx_t high = n;                 /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* Use seq_t to avoid overflow of the triangular number */
        seq_t tri = (seq_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    if (n == 0)                /* not needed for OEIS offset, but keep safe */
        return 0;

    idx_t r = find_row(n);                 /* row number */

    /* triangular number of preceding rows: (r-1)*r/2 */
    seq_t tri_prev = (seq_t)(r - 1) * r / 2;

    /* position inside the row: 0‑based index */
    seq_t pos = (seq_t)n - tri_prev - 1;

    seq_t mod = (seq_t)r;
    return (pos * 10) % mod;              /* k*10 mod n */
}
