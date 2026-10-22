/*
 * A362847  Triangle read by rows, T(n, k) = 4^k * Gamma(n + k + 1/2) /
 *          Gamma(n - k + 1/2).
 * https://oeis.org/A362847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) with T(r,k) = (2*(r+k)-1)!! / (2*(r-k)-1)!!, 0
 *            <= k <= r. Row r and column k are obtained from the zero‑based
 *            index n by solving r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n -
 *            r*(r+1)/2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 25, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row r for which r*(r+1)/2 <= n < (r+1)*(r+2)/2.  */
static inline idx_t _row_from_index(idx_t n)
{
    idx_t lo = 0, hi = 4294967295ULL; /* sqrt(2^64) ~ 4.3e9 */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k from the index n. */
    idx_t r = _row_from_index(n);
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;        /* first index of the row */
    idx_t k = n - start;                  /* column within the row */

    /* Compute T(r,k) = product_{i=r-k+1}^{r+k} (2*i-1). */
    seq_t result = 1;
    for (idx_t i = r - k + 1; i <= r + k; ++i) {
        result *= (seq_t)(2 * i - 1);
    }
    return result;
}
