/*
 * A326568  Denominator of the average of the multiset of prime indices of n.
 * https://oeis.org/A326568
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = k / gcd(S, k) where the multiset of prime indices of n is
 *            Σ e copies of the index of each prime factor p (with
 *            multiplicity e); S = Σ (index_of(p) * e), k = Σ e. The returned
 *            value is the denominator of the reduced average of this multiset
 * Range:     u8 n <= 7627, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 123 terms through n = 9944 (2026-11-03)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The prime indices of n with multiplicity have sum S and count k; a(n) is
   the denominator of S/k, k / gcd(S, k). pi comes from a sieve that grows
   with n (the original's table stopped at 20000). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static uint32_t *pi_tab = NULL;           /* pi_tab[x] = pi(x), x <= pi_lim */
static uint64_t pi_lim = 0;

static void ensure_pi(uint64_t n)
{
    if (n <= pi_lim) return;
    uint64_t lim = pi_lim ? 2 * pi_lim : 1024;
    while (lim < n) lim *= 2;
    unsigned char *comp = (unsigned char *)calloc(lim + 1, 1);
    uint32_t *t = (uint32_t *)malloc((lim + 1) * sizeof *t);
    if (!comp || !t) abort();
    for (uint64_t i = 2; i * i <= lim; ++i)
        if (!comp[i])
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
    uint32_t c = 0;
    for (uint64_t i = 0; i <= lim; ++i) {
        if (i >= 2 && !comp[i]) ++c;
        t[i] = c;
    }
    free(comp);
    free(pi_tab);
    pi_tab = t;
    pi_lim = lim;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    ensure_pi((uint64_t)n);
    uint64_t m = (uint64_t)n, S = 0, k = 0;
    for (uint64_t p = 2; p * p <= m; ++p)
        while (m % p == 0) {
            m /= p;
            S += pi_tab[p];
            ++k;
        }
    if (m > 1) { S += pi_tab[m]; ++k; }
    return (seq_t)k / gcd((seq_t)S, (seq_t)k);
}
