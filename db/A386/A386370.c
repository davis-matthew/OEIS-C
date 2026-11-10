/*
 * A386370  Square roots of partial sums of A386369 that are square numbers.
 * https://oeis.org/A386370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(A386688(n)). Instead of simulating A386369 term by
 *            term, jump from one square partial sum to the next. If S(k) =
 *            s^2, then A386369(j) = k for j > k until the next square, so the
 *            next square is s^2 + t*k = (s+d)^2, where d is the least d >= 1
 *            with d*(2s+d) == 0 (mod k). d is found by f...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-10)
 * Helpers:   addmod_u64, gcd_u64, is_prime_u64, modinv, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mulmod.h"
#include "../utils/addmod_u64.h"
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/modinv.h"

typedef unsigned __int128 A386370_u128;

static uint64_t A386370_rho(uint64_t n)
{
    if ((n & 1) == 0) return 2;
    for (uint64_t c = 1;; c++) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = addmod_u64(mulmod(x, x, n), c, n);
            y = addmod_u64(mulmod(y, y, n), c, n);
            y = addmod_u64(mulmod(y, y, n), c, n);
            d = gcd_u64(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}

static uint64_t A386370_ps[128];
static int A386370_np;

static void A386370_fac(uint64_t n)
{
    if (n == 1) return;
    if (is_prime_u64(n)) { A386370_ps[A386370_np++] = n; return; }
    uint64_t f = A386370_rho(n);
    A386370_fac(f);
    A386370_fac(n / f);
}

static uint64_t A386370_dp[64];
static int A386370_de[64];
static int A386370_nd;
static uint64_t A386370_k;
static A386370_u128 A386370_s2;
static uint64_t A386370_best;

static void A386370_dfs(int i, uint64_t u)
{
    if (i == A386370_nd) {
        uint64_t v = A386370_k / u;
        uint64_t c = (uint64_t)(A386370_s2 % v);
        c = (c == 0) ? 0 : v - c;
        uint64_t g = gcd_u64(u, v);
        if (c % g != 0) return;
        uint64_t vg = v / g;
        uint64_t lcm = u * vg;
        uint64_t x = 0;
        if (vg > 1) {
            uint64_t inv = modinv((u / g) % vg, vg);
            x = mulmod((c / g) % vg, inv, vg);
        }
        uint64_t d = u * x;
        if (d == 0) d = lcm;
        if (d < A386370_best) A386370_best = d;
        return;
    }
    uint64_t p = 1;
    for (int e = 0; e <= A386370_de[i]; e++) {
        A386370_dfs(i + 1, u * p);
        p *= A386370_dp[i];
    }
}

static seq_t A386370_to_seq(A386370_u128 x)
{
    uint64_t hi = (uint64_t)(x >> 64), lo = (uint64_t)x;
    seq_t r = (seq_t)lo;
    if (hi != 0) {
        seq_t h = (seq_t)hi;
        h <<= 32;
        h <<= 32;
        r += h;
    }
    return r;
}

seq_t a(idx_t n)
{
    static A386370_u128 *roots = NULL;
    static size_t cnt = 0, cap = 0;
    static A386370_u128 s = 0;
    static uint64_t k = 1;

    if (n < 1) return 0;
    if (roots == NULL) {
        cap = 64;
        roots = (A386370_u128 *)malloc(cap * sizeof(A386370_u128));
        roots[0] = 0;
        cnt = 1;
        s = 0;
        k = 1;
    }

    while (cnt < (size_t)n) {
        /* factor k */
        A386370_np = 0;
        uint64_t m = k;
        while ((m & 1) == 0) { A386370_ps[A386370_np++] = 2; m >>= 1; }
        for (uint64_t p = 3; p < 1000 && p * p <= m; p += 2)
            while (m % p == 0) { A386370_ps[A386370_np++] = p; m /= p; }
        A386370_fac(m);
        /* group into distinct primes with exponents */
        A386370_nd = 0;
        for (int i = 0; i < A386370_np; i++) {
            int j;
            for (j = 0; j < A386370_nd; j++)
                if (A386370_dp[j] == A386370_ps[i]) { A386370_de[j]++; break; }
            if (j == A386370_nd) {
                A386370_dp[A386370_nd] = A386370_ps[i];
                A386370_de[A386370_nd] = 1;
                A386370_nd++;
            }
        }
        A386370_k = k;
        A386370_s2 = s * 2;
        A386370_best = k;
        A386370_dfs(0, 1);

        uint64_t d = A386370_best;
        uint64_t g = gcd_u64(d, k);
        A386370_u128 t = (A386370_u128)(d / g) * ((s * 2 + d) / (k / g));
        s += d;
        k += (uint64_t)t;

        if (cnt == cap) {
            cap <<= 1;
            roots = (A386370_u128 *)realloc(roots, cap * sizeof(A386370_u128));
        }
        roots[cnt++] = s;
    }
    return A386370_to_seq(roots[(size_t)n - 1]);
}
