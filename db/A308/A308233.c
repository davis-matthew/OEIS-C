/*
 * A308233  Take apart the sides of each of the integer-sided triangles with
 *          perimeter n (at their vertices) and rearrange them orthogonally in
 *          3-space so that their endpoints coincide at a single point. a(n)
 *          is the total volume of all rectangular prisms enclosed in this
 *          way.
 * https://oeis.org/A308233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A308233
   Formula: a(n) = Sum_{k=1..floor(n/3)} Sum_{i=k..floor((n-k)/2)}
              sign(floor((i+k)/(n-i-k+1))) * i * k * (n-i-k)
*/
seq_t a(idx_t n)
{
    seq_t total = 0;
    if (n < 3) return 0;  /* early return for small n */

    idx_t k_max = n / 3;
    for (idx_t k = 1; k <= k_max; ++k) {
        idx_t i_max = (n - k) / 2;
        for (idx_t i = k; i <= i_max; ++i) {
            /* sign term is 1 iff i + k >= n - i - k + 1   */
            if (i + k >= n - i - k + 1) {
                seq_t term = (seq_t)i;
                term *= (seq_t)k;
                term *= (seq_t)(n - i - k);
                total += term;
            }
        }
    }
    return total;
}
