/*
 * A243506  Permutation of natural numbers: a(n) = A048673(A122111(n)).
 * https://oeis.org/A243506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048673(A122111(n)); computed from exponent vectors: with
 *            i_1<=...<=i_m the prime indices of n (with multiplicity), c_j =
 *            #{i_k >= j} for j=1..max, a(n) = (prod_j prime(c_j+1) + 1)/2
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 72, u64 n <= 180,
 *            u128 n <= 420, big any n
 * Validated: standard, pass, 80 terms through n = 1017 (2026-11-07)
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* small primes table: need prime(c+1) with c <= 64 */
    static uint32_t sp[100];
    static int spn = 0;
    if (spn == 0) {
        for (uint32_t x = 2; spn < 100; ++x) {
            bool ok = true;
            for (int i = 0; i < spn && (uint32_t)sp[i] * sp[i] <= x; ++i)
                if (x % sp[i] == 0) { ok = false; break; }
            if (ok) sp[spn++] = x;
        }
    }

    size_t total = 0;
    uint64_t maxidx = 0;
    uint64_t *pidx = (uint64_t *)malloc(pcnt * sizeof(uint64_t));
    if (!pidx) abort();
    for (size_t i = 0; i < pcnt; ++i) {
        pidx[i] = (uint64_t)prime_pi((idx_t)primes[i]);
        if (pidx[i] > maxidx) maxidx = pidx[i];
        total += (size_t)exps[i];
    }

    uint64_t *cnt = (uint64_t *)calloc((size_t)maxidx + 2, sizeof(uint64_t));
    if (!cnt) abort();
    for (size_t i = 0; i < pcnt; ++i)
        cnt[pidx[i]] += exps[i];
    /* suffix sums: cnt[j] = number of indices >= j */
    for (uint64_t j = maxidx; j >= 1; --j)
        cnt[j] += cnt[j + 1];

    seq_t prod = (seq_t)1;
    for (uint64_t j = 1; j <= maxidx; ++j)
        prod = prod * (seq_t)sp[cnt[j]];   /* prime number (c_j + 1) */

    free(cnt);
    free(pidx);
    (void)total;
    return (prod + (seq_t)1) / (seq_t)2;
}
