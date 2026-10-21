/*
 * A294082  Square array read by antidiagonals: T(m,n) = T(m,n-1)^2 -
 *          T(m,n-2)^2 + T(m,n-2) with T(1,n) = 1, T(m,0) = 1, and T(m,1) = m.
 * https://oeis.org/A294082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(m, ncoord) with T(m,0)=1, T(m,1)=m, T(m,n)=T(m,n-1)^2 -
 *            T(m,n-2)^2 + T(m,n-2). The array is read by antidiagonals: for
 *            each s=1,2,..., list T(m,s-m) for m=1..s. a(n) is the n-th term
 *            of this enumeration
 * Range:     u8 n <= 22, u16 n <= 29, u32 n <= 37, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the smallest s with s*(s+1)/2 >= n   (s >= 1). */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = ((seq_t)mid * (mid + 1)) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low; /* diagonal index */

    /* m is the position inside the diagonal (1-based). */
    idx_t m = n - ((s - 1) * s) / 2;
    /* ncoord is the column index for this m.  (m + ncoord = s). */
    idx_t ncoord = s - m;

    /* Compute T(m, ncoord) by recurrence. */
    if (ncoord == 0)
        return (seq_t)1;

    seq_t a_nm2 = (seq_t)1;           /* T(m,0) */
    seq_t a_nm1 = (seq_t)m;           /* T(m,1) */

    if (ncoord == 1)
        return a_nm1;

    seq_t a_n = a_nm1; /* placeholder */
    for (idx_t i = 2; i <= ncoord; ++i) {
        a_n = a_nm1 * a_nm1 - a_nm2 * a_nm2 + a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
