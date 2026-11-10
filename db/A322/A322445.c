/*
 * A322445  Smallest positive integer m such that n divides A297707(m).
 * https://oeis.org/A322445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{m>=1 : n | A297707(m)}, A297707(m) =
 *            m^(m-1)*Prod_{k=1..m-1} k^tau(m-k); search over m using prime
 *            exponents
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A322445_tau = NULL;
static uint64_t A322445_cap = 0;

static void A322445_ensure(uint64_t lim)
{
    if (lim <= A322445_cap) return;
    uint64_t nc = A322445_cap ? A322445_cap : 64;
    while (nc < lim) nc *= 2;
    free(A322445_tau);
    A322445_tau = (uint32_t *)calloc(nc + 1, sizeof(uint32_t));
    if (!A322445_tau) { A322445_cap = 0; return; }
    for (uint64_t i = 1; i <= nc; ++i)
        for (uint64_t j = i; j <= nc; j += i)
            ++A322445_tau[j];
    A322445_cap = nc;
}

static uint64_t A322445_vp(uint64_t x, uint64_t p)
{
    uint64_t c = 0;
    while (x % p == 0) { x /= p; ++c; }
    return c;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    uint64_t ps[20], es[20];
    int cnt = 0;
    uint64_t t = (uint64_t)n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            ps[cnt] = p; es[cnt] = e; ++cnt;
        }
    }
    if (t > 1) { ps[cnt] = t; es[cnt] = 1; ++cnt; }

    uint64_t start = ps[cnt - 1];
    A322445_ensure((uint64_t)n);
    if (!A322445_tau) return 0;

    for (uint64_t m = start; m <= (uint64_t)n; ++m) {
        int ok = 1;
        for (int i = 0; i < cnt && ok; ++i) {
            uint64_t p = ps[i];
            uint64_t E = A322445_vp(m, p) * (m - 1);
            for (uint64_t k = p; k < m && E < es[i]; k += p)
                E += A322445_vp(k, p) * (uint64_t)A322445_tau[m - k];
            if (E < es[i]) ok = 0;
        }
        if (ok) return (seq_t)m;
    }
    return (seq_t)n;
}
