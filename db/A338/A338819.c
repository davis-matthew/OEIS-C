/*
 * A338819  The entries in the rows of the n X n identity matrix, multiplied
 *          by the size of the matrix (n).
 * https://oeis.org/A338819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k if the n‑th term lies on the diagonal of the identity
 *            matrix of size k (i.e., n is such that (n-1) lies in the k×k
 *            block and its offset satisfies row==col); otherwise a(n)=0
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;          /* value is undefined for n=0,
                                               return 0 for safety */

    /* Find the matrix size k such that the cumulative number of
       entries up to k is at least n.
       The cumulative count is sum_{i=1}^k i^2. */
    idx_t k = 1;
    idx_t cum = 1;                         /* 1^2 */

    while (cum < n)
    {
        ++k;
        /* Use 128‑bit arithmetic to avoid overflow if k becomes large. */
        cum += (idx_t)((unsigned __int128)k * k);
    }

    /* cum now is sum_{i=1}^k i^2;
       prev is sum_{i=1}^{k-1} i^2. */
    idx_t prev   = cum - (idx_t)((unsigned __int128)k * k);
    idx_t offset = n - 1 - prev;           /* 0‑based index within the k×k block */

    idx_t row = offset / k;
    idx_t col = offset % k;

    return (row == col) ? (seq_t)k : (seq_t)0;
}
