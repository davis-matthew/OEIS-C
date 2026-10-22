/*
 * A350584  Triangle read by rows, T(n, k) = [x^k] ((2*x^3 - 3*x^2 - x + 1)/(1
 *          - x)^(n + 2)), for n >= 1 and 0 <= k < n.
 * https://oeis.org/A350584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangle read by rows, where T(n, k) = coefficient of x^k
 *            in ((2*x^3 - 3*x^2 - x + 1)/(1 - x)^(n + 2)), for n >= 1 and 0
 *            <= k < n. The sequence lists the rows consecutively
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for standard integer types */
#include "../utils/binomial.h" /* provides seq_t binomial(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    /* Find the row r such that sum_{i=1}^{r-1} i < n <= sum_{i=1}^{r} i. */
    idx_t r = 0, sum = 0;
    while (sum < n) {
        r++;
        sum += r;
    }
    idx_t prev_sum = sum - r;          /* number of terms before row r     */
    idx_t k = n - prev_sum - 1;        /* index within the row (0‑based)   */

    /* Compute T(r, k) = Σ_{t=0..3} coeff_t * C(r+1+(k-t), k-t)  where the
       terms are taken only if k >= t.  Coefficients are from the numerator
       1 - x - 3x^2 + 2x^3. */
    static const int coeff[4] = { 1, -1, -3, 2 };
    __int128 total = 0;
    for (int t = 0; t < 4; t++) {
        if (k < (idx_t)t) continue;
        idx_t a = r + 1 + (k - (idx_t)t);
        idx_t b = k - (idx_t)t;
        seq_t bin = binomial(a, b);
        total += (__int128)coeff[t] * (__int128)bin;
    }
    return (seq_t)total;
}
