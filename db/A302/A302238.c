/*
 * A302238  Expansion of Product_{k>=1} ((1 + x^k)/(1 - x^k))^prime(k).
 * https://oeis.org/A302238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * sum_{k=1}^{n} ( 2 * sum_{d|k, (k/d) odd} d * p_d
 *            ) * a(n-k) where p_d is the d-th prime (A000040(d)). This
 *            recurrence follows from the logarithm of the product G(x) =
 *            product_{k>=1} ((1 + x^k)/(1 - x^k))^{p_k}
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* allocate array for S(k) = 2 * sum_{d|k , odd m = k/d} d * p_d  (uint64_t) */
    uint64_t *S = (uint64_t*)calloc((size_t)(n + 1), sizeof(uint64_t));
    if (!S) return (seq_t)0;        /* fallback on allocation failure */

    /* compute S(k) */
    for (idx_t d = 1; d <= n; ++d) {
        uint64_t pd = get_prime(d);          /* p_d  (32‑bit)  */
        uint64_t val = 2ULL * (uint64_t)d * pd;   /* 2 * d * p_d  */
        for (uint64_t e = 1; d * e <= n; e += 2) { /* e odd */
            idx_t idx = d * e;
            S[idx] += val;
        }
    }

    /* dynamic programming for a(0..n) */
    seq_t *a = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!a) { free(S); return (seq_t)0; }

    a[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t prod = a[i - k] * (seq_t)S[k];
            sum += prod;
        }
        a[i] = sum / (seq_t)i;        /* division is exact */
    }

    seq_t res = a[n];
    free(S);
    free(a);
    return res;
}
