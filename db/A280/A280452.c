/*
 * A280452  G.f.: Product_{k>=1, j>=1} (1 + x^(j^2*k^2)).
 * https://oeis.org/A280452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of pairs (j,k) (j>=1,k>=1) such that
 *            sum_{(j,k)} (j*k)^2 = n. Equivalently, coefficient of x^n in
 *            ∏_{m≥1} (1 + x^{m^2})^{d(m)}, where d(m) is the number of
 *            divisors of m
 * Range:     u8 n <= 93, u16 n <= 310, u32 n <= 1164, u64 n <= 5008,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *dp_cache = NULL;
static idx_t dp_cache_size = 0;

seq_t a(idx_t n)
{
    if (n + 1 > dp_cache_size) {
        /* Recreate cache for current n */
        if (dp_cache) free(dp_cache);
        dp_cache_size = n + 1;
        dp_cache = (seq_t*)calloc(dp_cache_size, sizeof(seq_t));
        if (!dp_cache) return 0;
        dp_cache[0] = 1;

        for (idx_t m = 1; m * m <= n; ++m) {
            idx_t sq = m * m;

            /* Count divisors of m => number of copies of m^2 */
            idx_t d = 0;
            for (idx_t i = 1; i * i <= m; ++i) {
                if (m % i == 0) {
                    ++d;
                    if (i != m / i) ++d;
                }
            }

            /* Each copy is a 0/1 item of weight sq */
            for (idx_t copy = 0; copy < d; ++copy) {
                for (idx_t v = n; v >= sq; --v) {
                    dp_cache[v] += dp_cache[v - sq];
                }
            }
        }
    }
    return dp_cache[n];
}
