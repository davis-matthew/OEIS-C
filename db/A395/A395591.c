/*
 * A395591  Triangle read by rows: T(n,k) is the number of triangles formed in
 *          an n X k rectangular grid of unit squares with diagonals drawn in
 *          all squares, 1 <= k <= n.
 * https://oeis.org/A395591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   T(n,k) = * 4 * sum_{i=0}^{k-1} (n-i)*(k-i) * + 2 *
 *            sum_{i=0}^{min(k-1, floor((2*n-1)/4))} (k-i)*(2*n-1-4*i) * + 2 *
 *            sum_{i=0}^{floor((2*k-1)/4)} (n-i)*(2*k-1-4*i) * (triangle read
 *            by rows, a(n) = T(r,c) where r and c are found from n)
 * Range:     u8 n <= 9, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t triangle_T(idx_t n, idx_t k)
{
    seq_t sum1 = 0, sum2 = 0, sum3 = 0;
    seq_t sn = (seq_t)n;
    seq_t sk = (seq_t)k;

    /* First sum: i = 0 .. k-1 */
    for (idx_t i = 0; i < k; i++) {
        seq_t term = (sn - (seq_t)i) * (sk - (seq_t)i);
        sum1 += term;
    }

    /* Second sum limits */
    idx_t lim2 = k;
    {
        seq_t t = ((seq_t)2 * sn) - 1;
        idx_t max_i = (idx_t)(t / (seq_t)4);
        if (max_i < lim2) lim2 = max_i;
    }
    for (idx_t i = 0; i <= lim2; i++) {
        seq_t term = (sk - (seq_t)i) *
                     ((((seq_t)2 * sn) - 1) - (seq_t)4 * (seq_t)i);
        sum2 += term;
    }

    /* Third sum limits */
    {
        seq_t t = ((seq_t)2 * sk) - 1;
        idx_t max_i = (idx_t)(t / (seq_t)4);
        for (idx_t i = 0; i <= max_i; i++) {
            seq_t term = (sn - (seq_t)i) *
                         ((((seq_t)2 * sk) - 1) - (seq_t)4 * (seq_t)i);
            sum3 += term;
        }
    }

    seq_t result = (seq_t)4 * sum1 + (seq_t)2 * sum2 + (seq_t)2 * sum3;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not used in OEIS offset */

    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n) ++r;

    /* Column c = n - (r-1)*r/2 */
    idx_t prev = r * (r - 1) / 2;
    idx_t c = n - prev;

    return triangle_T(r, c);
}
