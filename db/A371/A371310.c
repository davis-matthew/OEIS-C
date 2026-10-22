/*
 * A371310  Expansion of e.g.f. Product_{k>=1} (1 + prime(k)*x^k/k!).
 * https://oeis.org/A371310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * coeff of x^n in ∏_{k≥1}(1 + prime(k)*x^k/k!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a_k(n) = a_{k-1}(n) + a_{k-1}(n-k)*prime(k)*C(n,k), */
#include <stdlib.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == (idx_t)0) return 1;
    idx_t N = n;
    seq_t *dp   = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *tmp  = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    dp[0] = 1;
    for (idx_t i = 1; i <= N; ++i) dp[i] = 0;

    for (idx_t k = 1; k <= N; ++k) {
        uint32_t pk = get_prime(k);

        /* copy values for indices < k (no change in this step) */
        for (idx_t i = 0; i < k; ++i) tmp[i] = dp[i];

        seq_t comb = 1;  /* C(k, k) = 1, will be updated for m>k */
        for (idx_t m = k; m <= N; ++m) {
            if (m > k)
                comb = comb * (seq_t)m / (seq_t)(m - k);

            seq_t prod = dp[m - k];
            prod = prod * (seq_t)pk;
            prod = prod * comb;

            tmp[m] = dp[m] + prod;
        }
        /* swap dp and tmp for next iteration */
        seq_t *swap = dp; dp = tmp; tmp = swap;
    }

    seq_t result = dp[N];
    free(tmp);
    free(dp);
    return result;
}
