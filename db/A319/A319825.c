/*
 * A319825  LCM of the strict integer partition with FDH number n.
 * https://oeis.org/A319825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   n = Product f(y_i) over its Fermi-Dirac prime factorization (f =
 *            A050376); a(n) = lcm(y_i), where y_i = index of p^(2^k) among
 *            the Fermi-Dirac primes = Sum_{j>=0} pi(floor(p^(2^k / 2^j)));
 *            a(1) = 0
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A319825_pi_tab = NULL;   /* pi(x) for 0 <= x <= A319825_pi_lim */
static uint64_t A319825_pi_lim = 0;

static void A319825_build_pi(uint64_t lim) {
    if (A319825_pi_tab && lim <= A319825_pi_lim) return;
    uint64_t newlim = lim < 1024 ? 1024 : lim;
    if (A319825_pi_tab && newlim < 2 * A319825_pi_lim) newlim = 2 * A319825_pi_lim;
    unsigned char *comp = (unsigned char *)calloc((size_t)newlim + 1, 1);
    uint32_t *tab = (uint32_t *)malloc(((size_t)newlim + 1) * sizeof(uint32_t));
    uint64_t i, j;
    for (i = 2; i * i <= newlim; i++)
        if (!comp[i])
            for (j = i * i; j <= newlim; j += i) comp[j] = 1;
    uint32_t c = 0;
    tab[0] = 0;
    if (newlim >= 1) tab[1] = 0;
    for (i = 2; i <= newlim; i++) {
        if (!comp[i]) c++;
        tab[i] = c;
    }
    free(comp);
    free(A319825_pi_tab);
    A319825_pi_tab = tab;
    A319825_pi_lim = newlim;
}

static uint64_t A319825_isqrt(uint64_t x) {
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

/* index of the Fermi-Dirac prime v in A050376 */
static uint64_t A319825_fd_index(uint64_t v) {
    A319825_build_pi(v);
    uint64_t cnt = 0, r = v;
    while (r >= 2) {
        cnt += A319825_pi_tab[r];
        r = A319825_isqrt(r);
    }
    return cnt;
}

static seq_t A319825_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    uint64_t m = (uint64_t)n;
    seq_t l = (seq_t)1;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p) continue;
        unsigned e = 0;
        while (m % p == 0) { m /= p; e++; }
        uint64_t v = p;
        while (e) {
            if (e & 1) {
                seq_t y = (seq_t)A319825_fd_index(v);
                l = l / A319825_gcd(l, y) * y;
            }
            e >>= 1;
            if (e) v = v * v;
        }
    }
    if (m > 1) {
        seq_t y = (seq_t)A319825_fd_index(m);
        l = l / A319825_gcd(l, y) * y;
    }
    return l;
}
