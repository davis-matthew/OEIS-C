/*
 * A259876  Triangle of numbers S(n,k) (0 <= k <= n) arising in the
 *          enumeration of interval orders without duplicated holdings.
 * https://oeis.org/A259876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 35, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the nth term of the flattened triangle T(n,k) defined by:
 *   T(n,n)   = (-1)^n,
 *   T(n,k)   = (2^n - 1) * T(n-1,k)   for 0 <= k < n
 * The sequence is obtained by listing the rows of T in order.
 */

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low  = 0;
    idx_t high = 1;
    /* double high until it is too large */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;                           /* row index */
    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);  /* first index of row r */
    idx_t k = n - start;                     /* column index (0 <= k <= r) */

    /* Diagonal element */
    if (k == r)
        return (r & 1) ? (seq_t)-1 : (seq_t)1;

    /* Off‑diagonal elements */
    seq_t prod = 1;
    seq_t pow2 = (seq_t)1 << (k + 1);   /* 2^(k+1) */

    for (idx_t j = k + 1; j <= r; ++j) {
        seq_t term = pow2 - 1;          /* 2^j - 1 */
        prod *= term;
        pow2 <<= 1;                     /* prepare 2^(j+1) for next iteration */
    }

    /* Multiply by (-1)^k */
    if (k & 1)
        prod = -prod;

    return prod;
}
