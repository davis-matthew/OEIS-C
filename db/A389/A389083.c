/*
 * A389083  Inverse map for A130789, where A130789 gives primes sorted
 *          according to decreasing ratio A001223(n)/A000040(n).
 * https://oeis.org/A389083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + #{primes q : (nextprime(q)-q)/q >
 *            (nextprime(p)-p)/p}, p = prime(n); i.e. rank of prime(n) in
 *            A130789 (sorted by decreasing A001223(k)/A000040(k)); computed
 *            by sieve, bounding gaps by log(q)^2
 * Range:     u8 n <= 82, u16 n <= 2062, u32 n <= 2062, u64 n <= 2062,
 *            u128 n <= 2062, big any n
 * Validated: standard, pass, 95 terms through n = 1991 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *inv_pr = NULL;
static size_t inv_cnt = 0;
static uint64_t inv_lim = 0;

static void inv_sieve(uint64_t lim)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    uint64_t i, j;
    size_t cnt = 0, k = 0;
    uint64_t *pr;
    if (!comp) return;
    for (i = 2; i * i <= lim; i++)
        if (!comp[i])
            for (j = i * i; j <= lim; j += i) comp[j] = 1;
    for (i = 2; i <= lim; i++) if (!comp[i]) cnt++;
    pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!pr) { free(comp); return; }
    for (i = 2; i <= lim; i++) if (!comp[i]) pr[k++] = i;
    free(comp);
    free(inv_pr);
    inv_pr = pr;
    inv_cnt = cnt;
    inv_lim = lim;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t lim = inv_lim ? inv_lim : 65536;
    for (;;) {
        if (lim != inv_lim) inv_sieve(lim);
        if (lim != inv_lim) return (seq_t)0;
        if ((uint64_t)inv_cnt > (uint64_t)n) {
            uint64_t p = inv_pr[n - 1];
            uint64_t g = inv_pr[n] - p;
            uint64_t bits = 0, t = lim;
            while (t) { bits++; t >>= 1; }
            uint64_t b = bits * 7 / 10 + 1;
            uint64_t B = b * b;
            if (B * p <= g * lim) {
                uint64_t rank = 1;
                size_t i;
                for (i = 0; i + 1 < inv_cnt; i++) {
                    uint64_t q = inv_pr[i];
                    uint64_t gq = inv_pr[i + 1] - q;
                    if (gq * p > g * q) rank++;
                }
                return (seq_t)rank;
            }
        }
        lim *= 2;
    }
}
