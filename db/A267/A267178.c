/*
 * A267178  Array read by antidiagonals: T(n,k) = parity of number of steps in
 *          simple Euclidean algorithm for gcd(n,k) where n >= 1, k >= 1.
 * https://oeis.org/A267178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = parity of the sum of quotients in the Euclidean algorithm
 *            for gcd(i,j), where (i,j) are the antidiagonal coordinates of n
 *            (1‑based)
 * Range:     u8 n <= 4950, u16 n <= 4950, u32 n <= 4950, u64 n <= 4950,
 *            u128 n <= 4950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static idx_t diag_of(idx_t n)
{
    /* Find the smallest d such that d*(d+1)/2 >= n.
       Binary search on the triangular numbers. */
    idx_t lo = 1, hi = n;
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Convert index n to coordinates (i,j) on the antidiagonal array. */
    idx_t d = diag_of(n);                     /* diagonal number (>=1) */
    idx_t prev = (unsigned __int128)(d - 1) * d / 2; /* number of terms before this diagonal */
    idx_t offset = n - prev - 1;              /* 0‑based offset within the diagonal */
    idx_t i = offset + 1;                     /* row index */
    idx_t j = d - offset;                     /* column index */

    /* Compute parity of the sum of quotients in the Euclidean algorithm. */
    seq_t parity = 0;
    idx_t a_val = i, b_val = j;
    while (b_val != 0)
    {
        idx_t q = a_val / b_val;
        parity ^= (q & 1);
        idx_t r = a_val % b_val;
        a_val = b_val;
        b_val = r;
    }
    return parity;
}
