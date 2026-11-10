/*
 * A282630  Number of steps to reach 1 when starting from n and iterating the
 *          map x -> x - A055396(x).
 * https://oeis.org/A282630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(n) = 1 + a(n - A055396(n)) for n > 1, where
 *            A055396(n) is the index of the smallest prime factor of n
 *            (computed by a sieve)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9591 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint32_t *A282630_steps = NULL;
static size_t A282630_cap = 0;

static void A282630_ensure(size_t n)
{
    if (n <= A282630_cap)
        return;
    size_t lim = n;
    if (lim < 2 * A282630_cap)
        lim = 2 * A282630_cap;
    if (lim < 1024)
        lim = 1024;

    uint32_t *spf = (uint32_t *)malloc((lim + 1) * sizeof(uint32_t));
    uint32_t *ord = (uint32_t *)malloc((lim + 1) * sizeof(uint32_t));
    uint32_t *st = (uint32_t *)malloc((lim + 1) * sizeof(uint32_t));
    if (!spf || !ord || !st) {
        free(spf); free(ord); free(st);
        return;
    }
    for (size_t i = 0; i <= lim; ++i) {
        spf[i] = 0;
        ord[i] = 0;
    }
    uint32_t cnt = 0;
    for (size_t i = 2; i <= lim; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            ord[i] = ++cnt;
            if (i * i <= lim) {
                for (size_t j = i * i; j <= lim; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }
    st[0] = 0;
    if (lim >= 1)
        st[1] = 0;
    for (size_t x = 2; x <= lim; ++x) {
        size_t y = x - ord[spf[x]];
        st[x] = st[y] + 1;
    }
    free(spf);
    free(ord);
    free(A282630_steps);
    A282630_steps = st;
    A282630_cap = lim;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    A282630_ensure((size_t)n);
    return (seq_t)A282630_steps[(size_t)n];
}
