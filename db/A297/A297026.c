/*
 * A297026  Positions of records in A297025.
 * https://oeis.org/A297026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 1; for n >= 3, a(n) = smallest y with
 *            A297025(y) = n-1, and a(n) <= 2*a(n-1). Found by a pruned search
 *            of the reverse A220096 tree (preimages of x: x+1 when it is
 *            prime, and x*p for primes p <= spf(x))
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static uint64_t *A297026_pr = NULL;
static size_t A297026_npr = 0;
static uint64_t A297026_plim = 0;

static void A297026_ensure_primes(uint64_t lim) {
    if (lim <= A297026_plim) return;
    if (lim < 1000) lim = 1000;
    unsigned char *c = (unsigned char *)calloc((size_t)lim + 1, 1);
    size_t cnt = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!c[i]) {
            cnt++;
            for (uint64_t j = i * i; j <= lim; j += i) c[j] = 1;
        }
    }
    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t k = 0;
    for (uint64_t i = 2; i <= lim; i++) if (!c[i]) pr[k++] = i;
    free(c);
    free(A297026_pr);
    A297026_pr = pr;
    A297026_npr = cnt;
    A297026_plim = lim;
}

static uint64_t A297026_isqrt(uint64_t v) {
    uint64_t r = 0;
    for (int b = 31; b >= 0; b--) {
        uint64_t t = r | ((uint64_t)1 << b);
        if (t * t <= v) r = t;
    }
    return r;
}

static uint64_t A297026_best;
static uint64_t A297026_L;
static unsigned A297026_T;

/* x has steps s and smallest prime factor sp */
static void A297026_dfs(uint64_t x, unsigned s, uint64_t sp) {
    if (s == A297026_T) {
        if (x <= A297026_L) {
            A297026_best = x;
            A297026_L = x - 1;
        }
        return;
    }
    unsigned t = A297026_T - s;
    unsigned sh = (t - 1) / 2; /* remaining ops after child: at least sh doublings */
    uint64_t lim = (sh >= 64) ? 0 : (A297026_L >> sh);
    if ((x & 1) == 0 && x + 1 <= lim && is_prime((idx_t)(x + 1)))
        A297026_dfs(x + 1, s + 1, x + 1);
    for (size_t i = 0; i < A297026_npr; i++) {
        uint64_t p = A297026_pr[i];
        if (p > sp) break;
        lim = (sh >= 64) ? 0 : (A297026_L >> sh);
        if (p > lim / x) break;
        A297026_dfs(x * p, s + 1, p);
    }
}

seq_t a(idx_t n) {
    static uint64_t rec[128];
    static unsigned cnt = 0;
    if (cnt == 0) {
        rec[0] = 0;
        rec[1] = 1;
        cnt = 2;
    }
    while (cnt < n && cnt < 128) {
        unsigned k = cnt; /* want smallest y with steps(y) = k */
        uint64_t L = 2 * rec[k - 1];
        A297026_ensure_primes(A297026_isqrt(L) + 2);
        A297026_L = L;
        A297026_T = k;
        A297026_best = L;
        A297026_dfs(2, 2, 2);
        rec[cnt++] = A297026_best;
    }
    return (seq_t)rec[n - 1];
}
