/*
 * A277004  Triangle read by rows, T(n,k) = k^k*(n-k)!*Sum_{j=0..n-k}(-1)^j/j!
 *          for 0<=k<=n.
 * https://oeis.org/A277004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 34, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277004 (triangle read by rows)
   a(n) = T(r,k) where
   T(r,k) = k^k * (r-k)! * sum_{j=0..r-k} (-1)^j / j!
   r = floor( (sqrt(8*n+1)-1)/2 )
   k = n - r*(r+1)/2
*/

static inline idx_t triangular(idx_t x)
{
    /* returns x*(x+1)/2 without overflow for x <= sqrt(2*UINT64_MAX) */
    if (x & 1)
        return x * ((x + 1) >> 1);
    else
        return (x >> 1) * (x + 1);
}

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (triangular(r + 1) <= n)
        ++r;

    /* position of n within the triangle */
    idx_t tri = triangular(r);          /* total elements before row r */
    idx_t k   = n - tri;                /* column index (0 <= k <= r) */
    idx_t d   = r - k;                  /* d = r - k, used in the recurrence */

    /* compute A(d) = d! * sum_{j=0..d} (-1)^j / j! via recurrence */
    seq_t A = (seq_t)1;                  /* A(0) = 1 */
    for (idx_t i = 1; i <= d; ++i) {
        if (i & 1) {                    /* odd i: subtract 1 */
            A = i * A;
            A -= (seq_t)1;
        } else {                        /* even i: add 1 */
            A = i * A + (seq_t)1;
        }
    }

    /* compute k^k (define 0^0 = 1) */
    seq_t pow_k = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        pow_k *= (seq_t)k;

    /* final result */
    return pow_k * A;
}
