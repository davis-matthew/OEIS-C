/*
 * A335340  North-East paths from (0,0) to (n,n) with k cyclic descents.
 * https://oeis.org/A335340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2 * binomial(r,k) * binomial(r-1,k-1) * where r =
 *            ceil((sqrt(8n+1)-1)/2) is the row containing the n‑th term, *
 *            and k = n - r*(r-1)/2 is the 1‑based column in that row
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the binomial coefficient C(n,k) using a multiplicative method. */
static seq_t binomial_simple(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* offset 1, guard for 0 */

    /* Find the row r such that r*(r+1)/2 ≥ n with binary search. */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;
    idx_t prev = (r - 1) * r / 2;     /* number of terms before row r */
    idx_t k = n - prev;               /* 1‑based column index */

    seq_t v1 = binomial_simple(r, k);          /* C(r,k) */
    seq_t v2 = binomial_simple(r - 1, k - 1);  /* C(r-1,k-1) */

    return (seq_t)2 * v1 * v2;
}
