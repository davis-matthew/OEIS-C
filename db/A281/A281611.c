/*
 * A281611  Expansion of Sum_{p prime, i>=2} x^(p^i)/(1 - x^(p^i)) /
 *          Product_{j>=1} (1 - x^j).
 * https://oeis.org/A281611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A073335(n) - A037032(n) (total number of proper
 *            prime‑power parts in all partitions of n)
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Partition numbers p[0..n] */
    seq_t *p = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!p) return 0;
    for (idx_t i = 0; i <= n; ++i) p[i] = 0;
    p[0] = 1;

    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            p[sum] = p[sum] + p[sum - part];
        }
    }

    /* Generate the primes up to n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);
    if (!primes) { free(p); return 0; }

    /* Collect all proper prime powers t = p^i, i >= 2, <= n */
    idx_t *prpow = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!prpow) { free(p); free(primes); return 0; }
    size_t prpow_cnt = 0;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t prime = (idx_t)primes[pi];
        if (prime * (idx_t)prime > n) continue;
        idx_t pw = prime * prime; /* starting at exponent 2 */
        while (pw <= n) {
            prpow[prpow_cnt++] = pw;
            if (pw > n / prime) break; /* avoid overflow of next power */
            pw *= prime;
        }
    }

    /* Sum over each proper prime power t: number of partitions of n having at
       least one part equal to t.  That equals sum_{k >= 1} p[n - k*t]. */
    seq_t ans = 0;
    for (size_t i = 0; i < prpow_cnt; ++i) {
        idx_t t = prpow[i];
        for (idx_t m = t; m <= n; m += t) {
            ans = ans + p[n - m];
        }
    }

    /* Clean up */
    free(p);
    free(prpow);
    free(primes);
    return ans;
}
