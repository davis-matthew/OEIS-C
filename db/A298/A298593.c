/*
 * A298593  Triangle read by rows: T(n,k) = number of times the value k
 *          appears on the parking functions of length n.
 * https://oeis.org/A298593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(m,k) where * m = smallest integer with m(m+1)/2 >= n *
 *            k = n - m(m-1)/2 * and T(m,k) is defined by * T(m,k) = m *
 *            Σ_{j=k..m} binom(m-1, j-1) * j^(j-2) * (m+1-j)^(m-1-j). *
 *            Exponents that are negative occur only when j = 1 (j^(j-2)) or j
 *            = m * (m+1-j)^(m-1-j); in those cases the value is...
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"     /* provides static seq_t binom(idx_t a, idx_t b) */
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* a(1) is the first term; n must be >= 1 for correct values */
    if (n == 0) {
        return 0;
    }

    /* Determine the row m such that 1+2+...+m >= n */
    idx_t m = 1;
    idx_t cum = 1;           /* cumulative number of entries up to current row */
    while (cum < n) {
        ++m;
        cum += m;
    }

    /* Column k within this row (1‑based) */
    idx_t k = n - (cum - m);

    /* Accumulate T(m,k) */
    seq_t result = 0;
    seq_t mm = (seq_t)m;     /* to avoid repeated casts */

    for (idx_t j = k; j <= m; ++j) {
        /* binomial coefficient */
        seq_t bin = binom(m - 1, j - 1);

        /* j^(j-2), with convention 1^(-1)=1 for j=1 */
        seq_t pow1;
        if (j <= 1) {
            pow1 = 1;
        } else {
            pow1 = big_pow((seq_t)j, (seq_t)(j - 2));
        }

        /* (m+1-j)^(m-1-j), with convention 1^(-1)=1 for j=m */
        seq_t pow2;
        if (j >= m) {
            pow2 = 1;
        } else {
            pow2 = big_pow((seq_t)(m + 1 - j), (seq_t)(m - 1 - j));
        }

        /* term = bin * pow1 * pow2 * m */
        seq_t term = bin;
        term *= pow1;
        term *= pow2;
        term *= mm;

        result += term;
    }

    return result;
}
