/*
 * A339701  Dirichlet g.f.: Product_{k>=2} (1 - k^(-s))^2.
 * https://oeis.org/A339701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *    Dirichlet generating function:
 *      A339701:  ∏_{k≥2} (1 – k⁻ˢ)²
 *
 *    The coefficient a(n) counts all ways of writing n as a product of
 *    integers ≥2 where each integer may be used at most twice.
 *    Using an integer once contributes a factor –2,
 *    using it twice contributes a factor +1.
 *    A dynamic‑programming routine enumerates these choices
 *    and sums the weights to obtain a(n) for every n.
 *
 *    Complexity:   O(n log n)   (≈ 1.1 × 10⁵ operations for n = 10 000)
 *    Memory:       O(n)         (≈ 8 kB for 10 000 64‑bit values)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the offset starts at 1 */
    /* array of coefficients a[1..n]; a[0] unused */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[1] = 1;                    /* base value a(1) = 1 */

    const seq_t minus_two = (seq_t)-2;   /* weight for a single use of k */

    /* process each integer k ≥ 2 once */
    for (idx_t k = 2; k <= n; ++k) {
        idx_t max_m = n / k;               /* largest m with m*k ≤ n */
        for (idx_t m = max_m; m >= 1; --m) {
            seq_t val = dp[m];              /* current coefficient for m */
            idx_t t1 = m * k;               /* product with k once */
            dp[t1] += val * minus_two;      /* weight –2 × val */

            idx_t t2 = t1 * k;              /* product with k twice  (k²) */
            if (t2 <= n) dp[t2] += val;     /* weight +1 × val */
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
