/*
 * A395722  Product of the distinct entries on the two main diagonals of the
 *          (2*n + 1) X (2*n + 1) square array formed by the integers 1, 2,
 *          ..., (2*n + 1)^2 in order.
 * https://oeis.org/A395722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = (2*n^2 + 2*n + 1) * Π_{k=1..n} (c^2 - 4*n^2*k^2) * (c^2 -
 *            4*(n+1)^2*k^2), where c = 2*n^2 + 2*n + 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the product of the distinct entries on the two main diagonals
   of the (2*n+1) × (2*n+1) array formed by 1,2,…,(2*n+1)^2. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nn   = (seq_t)n;
    seq_t np1  = nn + (seq_t)1;
    seq_t c    = (seq_t)2 * nn * nn + (seq_t)2 * nn + (seq_t)1;
    seq_t c2   = c * c;
    seq_t n_sq = nn * nn;
    seq_t np1_sq = np1 * np1;
    seq_t prod = c;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk   = (seq_t)k;
        seq_t k_sq = kk * kk;
        seq_t term1 = c2 - (seq_t)4 * n_sq * k_sq;
        seq_t term2 = c2 - (seq_t)4 * np1_sq * k_sq;
        prod *= term1;
        prod *= term2;
    }
    return prod;
}
