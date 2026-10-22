/*
 * A307982  G.f. A(x) satisfies: A(x) = x + x^2 + x^3 + x^4 * (1 + Sum_{i>=1}
 *          Sum_{j>=1} A(x^(i*j))).
 * https://oeis.org/A307982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=a(3)=a(4)=1; for n≥1, a(n+4) = Σ_{d|n} τ(n/d) * a(d)
 *            (τ = number of divisors)
 * Range:     u8 n <= 35, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Fast return for the initial zeros */
    if (n <= 4ULL) return 1;

    /* Static state: cache of computed a-values */
    static seq_t *a_cache = NULL;       /* array a[1..] */
    static size_t cache_cap = 0;        /* allocated size of a_cache   */
    static size_t cache_len = 0;        /* largest index already computed */

    /* Static state: divisor function τ[k] for 1 ≤ k ≤ max_needed */
    static unsigned int *tau = NULL;
    static size_t tau_cap = 0;          /* allocated size of tau array */

    /* Expand cache array if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Ensure the first four terms are always 1 */
    if (cache_len < 4) {
        a_cache[1] = a_cache[2] = a_cache[3] = a_cache[4] = 1;
        cache_len = 4;
    }

    /* Compute τ up to n‑4 if not already computed */
    size_t need_tau = (size_t)n - 4U;
    if (need_tau > tau_cap) {
        size_t new_tau_cap = need_tau;
        tau = (unsigned int *)realloc(tau, (new_tau_cap + 1) * sizeof(unsigned int));
        for (size_t i = 0; i <= new_tau_cap; ++i) tau[i] = 0;
        for (size_t i = 1; i <= new_tau_cap; ++i)
            for (size_t j = i; j <= new_tau_cap; j += i)
                tau[j] += 1;
        tau_cap = new_tau_cap;
    }

    /* Compute a(k) for k = cache_len+1 … n */
    for (size_t k = cache_len + 1; k <= (size_t)n; ++k) {
        size_t m = k - 4U;          /* m ≥ 1 */
        seq_t sum = 0;
        for (size_t i = 1; i <= m / i; ++i) {
            if (m % i == 0) {
                size_t d1 = i;
                size_t d2 = m / i;

                /* divisor d1 */
                size_t q1 = m / d1; /* q1 == d2 */
                sum += (seq_t)tau[q1] * a_cache[d1];

                if (d1 != d2) {
                    /* divisor d2 */
                    size_t q2 = m / d2; /* q2 == d1 */
                    sum += (seq_t)tau[q2] * a_cache[d2];
                }
            }
        }
        a_cache[k] = sum;
    }

    cache_len = (size_t)n;
    return a_cache[n];
}
