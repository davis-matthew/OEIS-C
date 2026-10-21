/*
 * A271917  Array read by antidiagonals: T(m,n) (m>=1, n>=1) = f(m,n) if m <=
 *          n or f(n,m) if n < m, where f(m,n) = m*(m^2-1)*(2*n-m)/12.
 * https://oeis.org/A271917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry when the array T(m,n) is read by
 *            antidiagonals. For m<=n, T(m,n)=m*(m^2-1)*(2*n-m)/12, otherwise
 *            T(m,n)=T(n,m). The sequence is OEIS A271917
 * Range:     u8 n <= 31, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find diagonal d such that (d-1)*d/2 >= n.  d=1 corresponds to point (1,0)
       which does not exist; the first real diagonal is d=2 for (1,1). */
    idx_t d = 2;
    /* The multiplication is safe for idx_t up to 64‑bits, since d is O(sqrt(n)). */
    while ((d - 1) * d / 2 < n) ++d;

    /* Number of terms before this diagonal. */
    idx_t prev = (d - 2) * (d - 1) / 2;
    /* 0‑based offset within diagonal. */
    idx_t pos  = n - prev - 1;

    /* Coordinates on diagonal d:
       m = i, n = d - i, where i = pos+1. */
    idx_t i = pos + 1;
    idx_t j = d - i;
    seq_t m = (seq_t)i;
    seq_t nCoord = (seq_t)j;

    /* Arrange so that m <= nCoord for formula. */
    seq_t mm = (m <= nCoord) ? m : nCoord;
    seq_t nn = (m <= nCoord) ? nCoord : m;

    /* Compute f(mm,nn) = mm*(mm^2-1)*(2*nn-mm)/12. */
    seq_t mmSq = mm * mm;
    seq_t numerator = mm * (mmSq - (seq_t)1) * (((seq_t)2 * nn) - mm);
    seq_t result = numerator / (seq_t)12;

    return result;
}
