/*
 * A334156  Triangle read by rows: T(n,m) is the number of length n decorated
 *          permutations avoiding the word 0^m = 0...0 of m 0's, where 1 <= m
 *          <= n.
 * https://oeis.org/A334156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{j=0}^{m-1} k! / j! , where k is the smallest integer
 *            with k*(k+1)/2 ≥ n, and m = n - k*(k-1)/2. This equals the
 *            flattened triangle of decorated permutations
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 78, u64 n <= 210,
 *            u128 n <= 562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety, offset 1 */

    /* find the row k */
    idx_t k = 1;
    while ((k * (k + 1)) / 2 < n)
        ++k;

    idx_t prev = (k - 1) * k / 2;   /* sum of previous rows */
    idx_t m = n - prev;             /* position in the row (1‑based) */

    /* compute Σ_{j=0}^{m-1} k! / j! by building terms backwards */
    seq_t term = 1;
    for (idx_t i = m; i <= k; ++i)
        term *= (seq_t)i;

    seq_t sum = 0;
    for (idx_t j = m; j > 0; --j) {
        sum += term;
        if (j > 1)
            term *= (seq_t)(j - 1);
    }
    return sum;
}
