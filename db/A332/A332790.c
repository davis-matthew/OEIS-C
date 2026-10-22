/*
 * A332790  Triangle read by rows: T(n,k) = 1 + 2*n + k + 5*k(n-k) for n >= 0,
 *          0 <= k <= n.
 * https://oeis.org/A332790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, and T(r,k) = 1 + 2*r + k + 5*k*(r-k)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row number r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2
       by binary search.  Use 128‑bit arithmetic to avoid overflow. */
    idx_t lo = 0;
    idx_t hi = n;                // upper bound: r can never exceed n
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        unsigned __int128 triangular = prod / 2;   // mid*(mid+1)/2
        if (triangular <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;

    /* Compute the starting index of this row (first term of row r) */
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;

    /* Column index within the row */
    idx_t k = n - start;

    /* Compute the value using seq_t arithmetic.  All intermediate values
       are cast to seq_t, ensuring that the calculations are performed
       in the target type. */
    seq_t val =
        (seq_t)1
        + (seq_t)2 * (seq_t)r
        + (seq_t)k
        + (seq_t)5 * (seq_t)k * (seq_t)(r - k);

    return val;
}
