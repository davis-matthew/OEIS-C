/*
 * A361974  (1,2)-block array, B(1,2), of the natural number array (A000027),
 *          read by descending antidiagonals.
 * https://oeis.org/A361974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the (1,2)-block array B(1,2) along
 *            descending antidiagonals. The block entry is b(i,j) = 2*i + (i +
 *            2*j - 2)^2. For a given index n≥1, the coordinates (i,j) are
 *            found from the sum s = i + j = ⌈(√(1+8n)−1)/2⌉ + 1 and the
 *            offset inside that antidiagonal
 * Range:     u8 n <= 36, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If called with n==0, return 0 (offset is 1). */
    if (n == 0) return 0;

    /* Find the smallest k such that k*(k+1)/2 ≥ n.
       Here k = s−1 where s is the antidiagonal sum i+j.   */
    idx_t k = 1;
    while ((k * (k + 1)) / 2 < n) ++k;

    idx_t s     = k + 1;                     /* i + j */
    idx_t t_prev = ((k - 1) * k) / 2;       /* terms before this diagonal */
    idx_t offset = n - t_prev - 1;           /* 0‑based offset inside the diagonal */

    idx_t i = 1 + offset;           /* row index (1‑based) */
    idx_t j = s - i;                /* column index (1‑based) */

    /* Compute b(i,j) = 2*i + (i + 2*j - 2)^2   */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t term = 2 * ii;
    seq_t t = ii + 2 * jj - 2;
    term += t * t;
    return term;
}
