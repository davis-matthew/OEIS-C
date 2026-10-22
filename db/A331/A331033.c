/*
 * A331033  a(n) is the least k such that the denominator(sigma(sigma(k))/k)
 *          equals n.
 * https://oeis.org/A331033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 2729, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): minimal k such that denominator(sigma(sigma(k))/k) == n.
   Computed by brute‑force search over multiples of n. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/bitlen_seq_t.h"

static seq_t sigma_of_u64(uint64_t n) {
    if (n <= 1) return (seq_t)n;
    size_t cnt = 0;
    uint64_t primes[32];
    uint64_t exps[32];
    factor_u64(n, primes, exps, &cnt);
    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term = pow_uint(p, (idx_t)(e + 1));
        term -= (seq_t)1;
        term /= (seq_t)(p - 1);
        res *= term;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for n = 0 */

    /* search over multiples k = n * m, m >= 1 */
    for (uint64_t m = 1;; ++m) {
        unsigned __int128 prod = (unsigned __int128)n * m;
        if (prod > (unsigned __int128)UINT64_MAX) break;   /* overflow of k */
        uint64_t k = (uint64_t)prod;

        /* sigma(k) */
        seq_t sigma_k = sigma_of_u64(k);
        if (bitlen_seq_t(sigma_k) > 64) continue;  /* cannot factor sigma(k) */

        uint64_t sigma_k_ll = (uint64_t)sigma_k;          /* fits in 64‑bit */

        /* sigma(sigma(k)) */
        seq_t sigma_sigma = sigma_of_u64(sigma_k_ll);

        /* gcd(k, sigma(sigma(k))) */
        seq_t g = gcd((seq_t)k, sigma_sigma);

        /* check k/g == n */
        seq_t k_div_g = (seq_t)k / g;
        if (k_div_g == (seq_t)n) return (seq_t)k;
    }

    /* If not found within 64‑bit bounds return 0 (should not happen). */
    return (seq_t)0;
}
