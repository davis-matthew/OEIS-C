/*
 * A346425  a(n) is the greatest number k such that k! <= prime(n).
 * https://oeis.org/A346425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{k : k! <= prime(n)}
 * Range:     u8 n <= 46, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute factorials up to 30! – ample for the prime values
       that arise in the tests (prime(10000) ≈ 1.0 × 10^5, 30! ≈ 2.6 × 10^32). */
    static seq_t fact[31];
    static bool init = false;
    const idx_t FACT_MAX = 30;

    if (!init) {
        fact[0] = 1;
        for (idx_t i = 1; i <= FACT_MAX; ++i)
            fact[i] = fact[i - 1] * i;
        init = true;
    }

    /* nth prime, 1‑based. */
    seq_t p = (seq_t)get_prime(n);

    /* Find the largest k with k! <= p.  k is an idx_t because it
       indexes the precomputed factorial array. */
    idx_t k = 0;
    while (k + 1 <= FACT_MAX && fact[k + 1] <= p)
        ++k;

    return (seq_t)k;
}
