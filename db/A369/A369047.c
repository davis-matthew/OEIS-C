/*
 * A369047  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A003415(i) = A003415(j) and A345000(i) = A345000(j), for all i,
 *          j >= 1.
 * https://oeis.org/A369047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of the pairs [A003415(n),
 *            A345000(n)]; computed by sieving pairs for 1..N and assigning
 *            ids in order of first appearance
 * Range:     u8 n <= 390, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 106 terms through n = 9840 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t A369047_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

static uint64_t A369047_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A369047_mulmod(r, b, m);
        b = A369047_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static uint64_t A369047_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A369047_deriv(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t res = 0, tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) { ++e; tmp /= p; }
            res += e * (n / p);
        }
    }
    if (tmp > 1) res += n / tmp;
    return res;
}

static uint64_t A369047_A345000(uint64_t n)
{
    static const uint64_t pr[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    if (n == 0) return 0;
    if (n == 1) return 1;
    uint64_t Dn = A369047_deriv(n);
    uint64_t prim[21];
    int k = 0;
    prim[0] = 1;
    while (k < 19) {
        unsigned __int128 nx = (unsigned __int128)prim[k] * pr[k];
        if (nx > n) break;
        prim[k + 1] = (uint64_t)nx;
        ++k;
    }
    int len = k + 1;
    uint64_t digits[21];
    uint64_t m = n;
    for (int i = k; i >= 0; --i) {
        digits[i] = m / prim[i];
        m -= digits[i] * prim[i];
    }
    uint64_t pw[21];
    for (int i = 0; i < len; ++i) pw[i] = A369047_powmod(pr[i], digits[i], Dn);
    uint64_t d2 = 0;
    for (int i = 0; i < len; ++i) {
        if (digits[i] == 0) continue;
        uint64_t t = A369047_powmod(pr[i], digits[i] - 1, Dn);
        for (int j = 0; j < len; ++j)
            if (j != i) t = A369047_mulmod(t, pw[j], Dn);
        t = A369047_mulmod(t, digits[i] % Dn, Dn);
        d2 = (d2 + t) % Dn;
    }
    return A369047_gcd(Dn, d2);
}

static uint64_t *A369047_ids = NULL;
static uint64_t A369047_cap = 0;

static void A369047_build(uint64_t cap)
{
    uint64_t *pd = (uint64_t *)malloc((cap + 1) * sizeof(uint64_t));
    uint64_t *pg = (uint64_t *)malloc((cap + 1) * sizeof(uint64_t));
    uint64_t *ids = (uint64_t *)malloc((cap + 1) * sizeof(uint64_t));
    uint64_t tsz = 1;
    while (tsz < 2 * cap) tsz <<= 1;
    uint64_t *tab = (uint64_t *)calloc(tsz, sizeof(uint64_t));
    uint64_t next = 0;
    for (uint64_t m = 1; m <= cap; ++m) {
        uint64_t d = A369047_deriv(m);
        uint64_t g = A369047_A345000(m);
        pd[m] = d; pg[m] = g;
        uint64_t h = (d * 0x9E3779B97F4A7C15ULL) ^ (g * 0xC2B2AE3D27D4EB4FULL);
        h ^= h >> 29;
        h &= tsz - 1;
        for (;;) {
            uint64_t e = tab[h];
            if (e == 0) {
                tab[h] = m;
                ids[m] = ++next;
                break;
            }
            if (pd[e] == d && pg[e] == g) { ids[m] = ids[e]; break; }
            h = (h + 1) & (tsz - 1);
        }
    }
    free(pd); free(pg); free(tab);
    if (A369047_ids) free(A369047_ids);
    A369047_ids = ids;
    A369047_cap = cap;
}

seq_t a(idx_t n)
{
    if (n > A369047_cap) {
        uint64_t cap = A369047_cap ? A369047_cap : 1024;
        while (cap < n) cap <<= 1;
        A369047_build(cap);
    }
    return (seq_t)A369047_ids[n];
}
