/*
 * A292564  Take 1, skip 3 * 1 - 1, take 2, skip 3 * 2 - 1, take 3, skip 3 * 3
 *          - 1, ...
 * https://oeis.org/A292564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((sqrt(8*n+1)-1)/2) *
 *            (3*floor((sqrt(8*n+1)-1)/2)+1)/2
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the integer k such that k*(k+1)/2 <= n < (k+1)*(k+2)/2.
   k is the largest integer with that property. */
static idx_t _triangular_root(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = 1;

    /* Double hi until it exceeds the bound. */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n)
        hi <<= 1;

    lo = hi >> 1;       /* hi is now strictly greater than needed */
    /* Binary search between lo and hi. */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t k = _triangular_root(n);
    seq_t ks = (seq_t)k;
    seq_t term = ks * ((seq_t)3 * ks + 1) / 2;
    return (seq_t)n + term;
}
