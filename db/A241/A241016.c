/*
 * A241016  Triangle read by rows: T(n, k) = sum of k-th row of n X n square
 *          filled with the numbers 1 through n^2 reading across rows
 *          left-to-right.
 * https://oeis.org/A241016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,c) where r is the row such that * (r‑1)*r/2 < n ≤
 *            r*(r+1)/2, and c = n – (r‑1)*r/2. * Then the triangle entry is *
 *            T(r,c) = r*(r+1)/2 + r²*(c‑1)
 * Range:     u8 n <= 26, u16 n <= 859, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row r for the given linear index n (n ≥ 1).
 * r satisfies r*(r+1)/2 ≥ n and (r-1)*r/2 < n. */
static inline idx_t
row_of(idx_t n)
{
    idx_t low = 1;
    idx_t high = n;           /* high = n gives an upper bound */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Step 1: locate row r */
    idx_t r = row_of(n);

    /* Step 2: determine column c */
    unsigned __int128 prev_tri128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev_tri = (idx_t)prev_tri128;
    idx_t c = n - prev_tri;   /* 1 ≤ c ≤ r */

    /* Step 3: compute the triangle entry */
    seq_t row_sum = ((seq_t)r * (r + 1)) / 2;               /* r*(r+1)/2 */
    seq_t r_sq   = (seq_t)r * (seq_t)r;                     /* r² */
    seq_t val    = row_sum + r_sq * ((seq_t)c - 1);          /* complete formula */

    return val;
}
