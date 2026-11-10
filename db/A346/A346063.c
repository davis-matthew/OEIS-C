/*
 * A346063  a(n) = primepi(A039634(prime(n)^2-1)).
 * https://oeis.org/A346063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000720(A039634(A000040(n)^2-1)); computed with a sieve
 *            for prime(n), a binary-prefix primality scan for A039634, and
 *            Lucy_Hedgehog prime counting for pi(x)
 * Range:     u8 n <= 55, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

static uint64_t a346063_nth_prime(uint64_t n)
{
    uint64_t lim = 64;
    for (;;) {
        unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        uint64_t i, j, cnt = 0;
        if (!comp) return 0;
        for (i = 2; i * i <= lim; i++)
            if (!comp[i])
                for (j = i * i; j <= lim; j += i) comp[j] = 1;
        for (i = 2; i <= lim; i++) {
            if (!comp[i]) {
                cnt++;
                if (cnt == n) { free(comp); return i; }
            }
        }
        free(comp);
        lim *= 2;
    }
}

static uint64_t a346063_isqrt(uint64_t x)
{
    uint64_t lo = 0, hi = 4294967296ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= x) lo = mid; else hi = mid - 1;
    }
    return lo;
}

static uint64_t a346063_primepi(uint64_t x)
{
    if (x < 2) return 0;
    uint64_t r = a346063_isqrt(x), i, p;
    uint64_t *lo = (uint64_t *)malloc((r + 2) * sizeof(uint64_t));
    uint64_t *hi = (uint64_t *)malloc((r + 2) * sizeof(uint64_t));
    uint64_t res;
    if (!lo || !hi) { free(lo); free(hi); return 0; }
    lo[0] = 0; hi[0] = 0;
    for (i = 1; i <= r; i++) { lo[i] = i - 1; hi[i] = x / i - 1; }
    for (p = 2; p <= r; p++) {
        if (lo[p] == lo[p - 1]) continue;
        uint64_t pc = lo[p - 1], p2 = p * p, end = x / p2;
        if (end > r) end = r;
        for (i = 1; i <= end; i++) {
            uint64_t d = i * p;
            hi[i] -= (d <= r ? hi[d] : lo[x / d]) - pc;
        }
        for (i = r; i >= p2; i--)
            lo[i] -= lo[i / p] - pc;
    }
    res = hi[1];
    free(lo); free(hi);
    return res;
}

/* largest prime that is a binary prefix of m (1 if none) */
static uint64_t a346063_prefix_prime(uint64_t m)
{
    int len = 0, i;
    uint64_t t = m, pref = 0, best = 1;
    while (t) { len++; t >>= 1; }
    for (i = 0; i < len; i++) {
        pref = (pref << 1) | ((m >> (len - 1 - i)) & 1ULL);
        if (pref > 1 && is_prime_u64(pref)) best = pref;
    }
    return best;
}

seq_t a(idx_t n)
{
    uint64_t p = a346063_nth_prime((uint64_t)n);
    uint64_t m = p * p - 1;
    uint64_t q = a346063_prefix_prime(m);
    return (seq_t)a346063_primepi(q);
}
