/*
 * A291783  Partial sums of A064415(k)^2.
 * https://oeis.org/A291783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A064415(k)^2, where A064415 is completely
 *            additive with A064415(1)=0, A064415(2)=1,
 *            A064415(p)=1+A064415((p-1)/2) for odd primes p
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *ps_pref = NULL;
static idx_t ps_lim = 0;

static void ps_build(idx_t lim) {
    size_t N = (size_t)lim;
    uint32_t *spf = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t *f = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    if (!spf || !f) exit(1);
    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            for (size_t j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    f[1] = 0;
    for (size_t m = 2; m <= N; ++m) {
        size_t p = spf[m];
        if (p == m) {
            if (m == 2) f[m] = 1;
            else f[m] = 1 + f[(m - 1) / 2];
        } else {
            f[m] = f[p] + f[m / p];
        }
    }
    if (ps_pref) mem_free(ps_pref);
    ps_pref = mem_new(seq_t, N + 1);
    seq_t s = 0;
    ps_pref[0] = 0;
    for (size_t k = 1; k <= N; ++k) {
        s += (seq_t)f[k] * (seq_t)f[k];
        ps_pref[k] = s;
    }
    ps_lim = lim;
    free(spf);
    free(f);
}

seq_t a(idx_t n) {
    if (n > ps_lim) {
        idx_t want = ps_lim * 2;
        if (want < n) want = n;
        if (want < 64) want = 64;
        ps_build(want);
    }
    return ps_pref[(size_t)n];
}
