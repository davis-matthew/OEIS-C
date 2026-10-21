/*
 * A268732  Sum of the numbers of divisors of gcd(x,y) with x*y <= n.
 * https://oeis.org/A268732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} τ(k) * floor(sqrt(n/k))
 * Range:     u8 n <= 49, u16 n <= 5243, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static inline idx_t isqrt_u64(idx_t x) {
    /* Compute ⌊√x⌋ for unsigned 64‑bit integers. */
    idx_t r = x;
    while (r * r > x) {
        r = (r + x / r) >> 1;
    }
    while ((r + 1) * (r + 1) <= x) {
        ++r;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    static uint64_t *tau = NULL;
    static idx_t tau_cap = 0;

    if (n > tau_cap) {
        uint64_t *new_tau = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
        if (!new_tau) return 0;  /* allocation failure – should not happen */

        for (idx_t i = 0; i <= n; ++i) new_tau[i] = 0;
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i) {
                new_tau[j]++;
            }
        }

        free(tau);
        tau = new_tau;
        tau_cap = n;
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t tau_k = (seq_t)tau[k];
        idx_t t = n / k;
        idx_t sqrt_t = isqrt_u64(t);
        sum += tau_k * (seq_t)sqrt_t;
    }

    return sum;
}
