/*
 * A274824  Triangle read by rows: T(n,k) = (n-k+1)*sigma(k), n>=1, 1<=k<=n.
 * https://oeis.org/A274824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (r - k + 1) * σ(k), where r is the smallest integer *
 *            with r*(r + 1)/2 ≥ n, k = n - (r - 1)*r/2 (1‑based column), and
 *            * σ(k) is the sum of the positive divisors of k
 * Range:     u8 n <= 221, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative number of entries up to that row
       is at least n.  r*(r+1)/2 is the number of entries in the first r rows. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* k is the position within the r‑th row (1‑based). */
    idx_t k = n - (r - 1) * r / 2;

    /* Compute σ(k) (sum of divisors of k). */
    seq_t sigma = 0;
    for (idx_t i = 1; i <= k / i; ++i) {
        if (k % i == 0) {
            sigma += (seq_t)i;
            if (i != k / i)
                sigma += (seq_t)(k / i);
        }
    }

    /* Compute and return the triangle entry. */
    seq_t result = (seq_t)(r - k + 1) * sigma;
    return result;
}
