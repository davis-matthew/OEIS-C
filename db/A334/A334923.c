/*
 * A334923  Square array T(n,k) = ((5/2)*n*k - (1/2)*A319929(n,k))/2, n >= 1,
 *          k >= 1, read by antidiagonals.
 * https://oeis.org/A334923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 390, u16 n <= 465, u32 n <= 465, u64 n <= 465,
 *            u128 n <= 465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A334923: a(n) is obtained by reading the square array
    T(i,j) = i*j + floor(i/2)*floor(j/2) along the anti‑diagonals
    of the array.  For a given n, let d be the smallest integer with
    d*(d+1)/2 >= n (the anti‑diagonal number).  Then
    i = n - d*(d-1)/2   (1 ≤ i ≤ d)
    j = d - i + 1
    and a(n) = i*j + (i/2)*(j/2)   (integer division).  */
#include <stdint.h>

static inline idx_t find_diag(idx_t n)
{
    /* Find minimal d with d*(d+1)/2 >= n */
    idx_t lo = 1, hi = 1;
    /* Expand hi until it is large enough */
    while ((unsigned __int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;
    /* Binary search */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* 0 is outside the OEIS offset; not used */
        return 0;
    idx_t d = find_diag(n);                     /* anti‑diagonal number */
    idx_t prev = (unsigned __int128)(d - 1) * d / 2; /* number of terms before this diagonal */
    idx_t offset = n - prev - 1;                /* 0‑based position inside diagonal */

    idx_t i = offset + 1;                       /* row index   */
    idx_t j = d - offset;                       /* column index */

    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t val = ii * jj + (ii / 2) * (jj / 2);
    return val;
}
