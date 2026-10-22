/*
 * A328826  Triangle read by rows: binomial(n,k)*(2*n-k)!, n>=0, 0<=k<=n.
 * https://oeis.org/A328826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) enumerates T(r,k)=binomial(r,k)*(2*r‑k)! by rows. r is the
 *            largest integer such that r*(r+1)/2 ≤ n, and k = n – r*(r+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 27, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find row r */
    idx_t r = 0;
    while ( ((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n )
        ++r;

    idx_t k = n - r * (r + 1) / 2;

    /* compute binomial(r,k) */
    seq_t binom = 1;
    for (idx_t i = 1; i <= k; ++i)
        binom = binom * (r - k + i) / i;

    /* compute factorial (2*r - k)! */
    seq_t fact = 1;
    idx_t limit = 2 * r - k;
    for (idx_t j = 2; j <= limit; ++j)
        fact = fact * j;

    return binom * fact;
}
