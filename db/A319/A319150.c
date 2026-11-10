/*
 * A319150  a(n) = gcd(A275286(n), A001818(n+1)) / A025549(n+1)^2.
 * https://oeis.org/A319150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A275286(n), A001818(n+1)) / A025549(n+1)^2 * = gcd(N,
 *            L^2), where L = lcm{1,3,5,...,2n+1} and * N = L^2 * Sum_{i=0..n}
 *            (-1)^i/(2i+1)^2 = Sum_{i=0..n} (-1)^i*(L/(2i+1))^2. * Computed
 *            prime by prime: for each odd prime p with p^e || L, * the p-part
 *            is p^min(v_p(N), 2e), found from N mod...
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Validated: partial, pass, 95 terms through n = 94 (2026-11-08)
 * Helpers:   mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mulmod.h"

static uint64_t A319150_inv_mod(uint64_t a, uint64_t m)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)m, newr = (int64_t)(a % m);
    while (newr != 0) {
        int64_t q = r / newr, tmp;
        tmp = t - q * newt; t = newt; newt = tmp;
        tmp = r - q * newr; r = newr; newr = tmp;
    }
    if (t < 0) t += (int64_t)m;
    return (uint64_t)t;
}

seq_t a(idx_t n)
{
    uint64_t M = 2 * (uint64_t)n + 1;
    seq_t res = 1;
    if (M < 3) return res;

    unsigned char *comp = (unsigned char *)calloc((size_t)M + 1, 1);
    for (uint64_t i = 2; i * i <= M; i++)
        if (!comp[i])
            for (uint64_t j = i * i; j <= M; j += i) comp[j] = 1;

    for (uint64_t p = 3; p <= M; p += 2) {
        if (comp[p]) continue;
        uint64_t pe = 1;
        int e = 0;
        while (pe <= M / p) { pe *= p; e++; }
        uint64_t m = pe * pe;

        /* Lp = L / p^e mod m */
        uint64_t Lp = 1 % m;
        for (uint64_t q = 3; q <= M; q += 2) {
            if (comp[q] || q == p) continue;
            uint64_t qe = 1;
            while (qe <= M / q) qe *= q;
            Lp = mulmod(Lp, qe % m, m);
        }

        uint64_t N = 0;
        for (uint64_t x = 1; x <= M; x += 2) {
            uint64_t j = x;
            int v = 0;
            while (j % p == 0) { j /= p; v++; }
            uint64_t t = mulmod(Lp, A319150_inv_mod(j % m, m), m);
            for (int k = v; k < e; k++) t = mulmod(t, p % m, m);
            t = mulmod(t, t, m);
            if (((x - 1) / 2) & 1)
                N = (N + m - t) % m;
            else
                N = (N + t) % m;
        }

        int cnt = 0;
        if (N == 0) {
            cnt = 2 * e;
        } else {
            while (N % p == 0) { N /= p; cnt++; }
        }
        for (int k = 0; k < cnt; k++) res = res * (seq_t)p;
    }
    free(comp);
    return res;
}
