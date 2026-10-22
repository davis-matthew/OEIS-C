/*
 * A327259  Array T(n,k) = 2*n*k - A319929(n,k), n >= 1, k >= 1, read by
 *          antidiagonals.
 * https://oeis.org/A327259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the anti-diagonal enumeration of the table T(n,k)
 *            defined by: T(n,k) = 2*n*k - n - k + 1 if n odd && k odd T(n,k)
 *            = 2*n*k - n if n even && k odd T(n,k) = 2*n*k - k if n odd && k
 *            even T(n,k) = 2*n*k if n even && k even The sequence a is read
 *            by antidiagonals in order (1,1), (1,2),(2,1), (1...
 * Range:     u8 n <= 260, u16 n <= 465, u32 n <= 465, u64 n <= 465,
 *            u128 n <= 465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find diagonal number m such that m*(m-1)/2 >= n */
    idx_t m = 2;
    while ((unsigned __int128)m * (m - 1) / 2 < n)
        ++m;

    /* compute index within this diagonal (0‑based) */
    unsigned __int128 prev_val = (unsigned __int128)(m - 2) * (m - 1) / 2;
    idx_t prev = (idx_t)prev_val;
    idx_t off   = n - prev - 1;          /* 0‑based offset in the diagonal */

    idx_t i = off + 1;   /* row index (1‑based)   */
    idx_t j = m - i;     /* column index (1‑based) */

    /* compute T(i,j) */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t two_ij = ii * jj * 2;

    if ((i & 1) && (j & 1))                 /* both odd */
        return two_ij - ii - jj + 1;
    else if ((i & 1) == 0 && (j & 1))       /* i even, j odd */
        return two_ij - ii;
    else if ((i & 1) && (j & 1) == 0)       /* i odd, j even */
        return two_ij - jj;
    else                                   /* both even */
        return two_ij;
}
