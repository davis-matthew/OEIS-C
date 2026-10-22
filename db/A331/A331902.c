/*
 * A331902  T(n, k) = floor(n/m) where m is the least positive integer such
 *          that floor(n/m) = floor(k/m). Square array read by antidiagonals,
 *          for n >= 0 and k >= 0.
 * https://oeis.org/A331902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j), where (i,j) are the coordinates on the *
 *            antidiagonal of n in the square array defined by OEIS A331902. *
 *            T(i,j) = floor(i/m) where m is the smallest positive integer
 *            such that * floor(i/m) = floor(j/m). The array is symmetric:
 *            T(i,j) = T(j,i)
 * Range:     u8 n <= 9677, u16 n <= 9677, u32 n <= 9677, u64 n <= 9677,
 *            u128 n <= 9677, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation below follows the definition directly.
 * It computes the antidiagonal coordinates in O(1) time and then
 * finds the required m by a simple scan – for the small values
 * that occur (i and j ≤ ≈√(2n)), this is well under 2 s even for n≈10 000. */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned values.
 * Returns ⌊√x⌋.  The routine never overflows because the
 * search range is limited to 0 .. 0xFFFFFFFF. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    /* √(2^64-1) ≈ 2^32-1, so we never need to search above 0xFFFFFFFF. */
    idx_t hi = (x < 0xFFFFFFFFull) ? x : 0xFFFFFFFFull;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Convert a linear index n into the coordinates (i,j) of the
 * corresponding entry on the antidiagonal.  The array is read by
 * antidiagonals: n = 0 → (0,0),
 * n = 1 → (0,1), n = 2 → (1,0), n = 3 → (0,2), …  */
static inline void antidiag_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* Solve s*(s+1)/2 <= n < (s+1)*(s+2)/2.
     * s is the diagonal index containing n. */
    idx_t diag = (isqrt_u64(8 * n + 1) - 1) / 2;
    idx_t prev = diag * (diag + 1) / 2;   /* first index on this diagonal */
    idx_t offset = n - prev;              /* offset along the diagonal   */
    *i = offset;                          /* row index   */
    *j = diag - offset;                   /* column index */
}

/* Compute T(i,j) for i ≤ j.  The function allows i==j, in which case
 * it simply returns i (which equals j).  For i<j it scans m from 1 to j
 * and returns the first m such that floor(i/m) == floor(j/m);  if no
 * such m exists in that range, T(i,j) = 0.  This matches the behaviour
 * of the OEIS definition where for max(i,j) ≥ 2*min(i,j) a zero occurs. */
static inline seq_t T_val(idx_t i, idx_t j)
{
    if (i == j)
        return (seq_t)i;          /* T(i,i) = i (or 0 when i==0) */

    /* Ensure i < j for the scanning loop. */
    if (i > j) {
        idx_t tmp = i;
        i = j;
        j = tmp;
    }

    for (idx_t m = 1; m <= j; ++m) {
        seq_t a = (seq_t)(i / m);
        seq_t b = (seq_t)(j / m);
        if (a == b)
            return a;
    }
    return (seq_t)0;               /* no suitable m found */
}

/* Public entry point required by the test harness. */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiag_coords(n, &i, &j);
    return T_val(i, j);
}
