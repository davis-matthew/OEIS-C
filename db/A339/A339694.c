/*
 * A339694  Triangle read by rows: A(n, k) = Sum_{i=0..n-1} x(i, k)*2^i, where
 *          x(i, k) = A014682^(i)(k) (mod 2) using the i-th iteration of
 *          A014682.
 * https://oeis.org/A339694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A(n, k) for the index k = n - (2^r - 2) - 1, * where r =
 *            floor(log2(n+1)), and * A(n, k) = Σ_{i=0}^{r-1} (B_i(k) mod 2) *
 *            2^i, * B_0(k) = k, B_{i+1}(x) = x/2 if x even * = (3x+1)/2 if x
 *            odd. * This implements OEIS A339694
 * Range:     u8 n <= 511, u16 n <= 9117, u32 n <= 9117, u64 n <= 9117,
 *            u128 n <= 9117, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* floor(log2(v)) for v > 0 */
static idx_t floor_log2_idx(idx_t v)
{
    idx_t r = 0;
    while ((v >> 1U) != 0U) {
        v >>= 1U;
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* row r: smallest integer with n <= 2^(r+1) - 2 */
    idx_t r = floor_log2_idx(n + 1U);        /* r >= 1 for n >= 1 */

    /* number of terms preceding this row: 2^r - 2 */
    idx_t prev = ((idx_t)1 << r) - 2U;

    /* k is the 0‑based index within the current row */
    idx_t k = n - prev - 1U;

    seq_t val = (seq_t)k;  /* current state of the short Collatz map */
    seq_t ans = 0;

    for (idx_t i = 0; i < r; ++i) {
        seq_t bit = val & 1U;          /* parity of current value */
        ans += (seq_t)bit << i;        /* add contribution 2^i * bit */

        /* advance one short Collatz step:
         *   even -> x / 2
         *   odd  -> (3 x + 1) / 2
         */
        if ((val & 1U) == 0U) {
            val >>= 1U;
        } else {
            val = ((val * 3U) + 1U) >> 1U;
        }
    }
    return ans;
}
