/*
 * A373152  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A085731(i) = A085731(j) and A373145(i) = A373145(j), for all i,
 *          j >= 1.
 * https://oeis.org/A373152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pair *
 *            [gcd(n, A003415(n)), gcd(A003415(n), A276085(n))], computed by a
 *            sieve. * A276085(n) can be far too large for 64 bits, so the
 *            code uses * gcd(D, S) = gcd(D, S mod D) with D = A003415(n) and
 *            works out S mod D directly
 * Range:     u8 n <= 2260, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9904 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *rg_res = NULL;
static idx_t rg_N = 0;

static uint64_t rg_gcd64(uint64_t x, uint64_t y) {
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static uint64_t rg_mulmod(uint64_t x, uint64_t y, uint64_t m) {
    return (uint64_t)(((unsigned __int128)x * y) % m);
}

static void rg_build(idx_t N) {
    size_t NN = (size_t)N;
    uint32_t *spf = (uint32_t *)malloc(sizeof(uint32_t) * (NN + 2));
    uint32_t *primes = (uint32_t *)malloc(sizeof(uint32_t) * (NN + 2));
    size_t np = 0;
    uint64_t *G1 = (uint64_t *)malloc(sizeof(uint64_t) * (NN + 2));
    uint64_t *G2 = (uint64_t *)malloc(sizeof(uint64_t) * (NN + 2));
    size_t HS = 2 * NN + 1;
    int *head = (int *)malloc(sizeof(int) * HS);
    int *nxt = (int *)malloc(sizeof(int) * (NN + 2));
    uint32_t *res = (uint32_t *)malloc(sizeof(uint32_t) * (NN + 2));
    size_t i, j;
    for (i = 0; i <= NN + 1; i++) spf[i] = 0;
    for (i = 2; i <= NN; i++) {
        if (spf[i] == 0) {
            primes[np++] = (uint32_t)i;
            for (j = i; j <= NN; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 0; i < HS; i++) head[i] = -1;
    int cnt = 0;
    res[0] = 0;
    for (i = 1; i <= NN; i++) {
        /* factor i */
        uint32_t fp[64];
        int fe[64];
        int fc = 0;
        size_t m = i;
        while (m > 1) {
            uint32_t p = spf[m];
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            fp[fc] = p; fe[fc] = e; fc++;
        }
        uint64_t D = 0;
        for (int k = 0; k < fc; k++)
            D += (uint64_t)(i / fp[k]) * (uint64_t)fe[k];
        uint64_t g1 = rg_gcd64((uint64_t)i, D);
        uint64_t g2;
        if (D == 0) {
            g2 = 0; /* i = 1: gcd(0, 0) */
        } else if (D == 1) {
            g2 = 1;
        } else {
            /* S mod D, S = sum e * (product of primes < p) */
            uint64_t S = 0, prod = 1 % D;
            size_t pi = 0;
            for (int k = 0; k < fc; k++) { /* fp ascending */
                uint32_t p = fp[k];
                while (primes[pi] < p) {
                    prod = rg_mulmod(prod, primes[pi] % D, D);
                    pi++;
                }
                S = (S + rg_mulmod((uint64_t)fe[k] % D, prod, D)) % D;
            }
            g2 = rg_gcd64(D, S);
        }
        size_t b = (size_t)(((g1 % 1000003ULL) * 1000033ULL + (g2 % 1000003ULL)) % HS);
        int found = -1;
        for (int k = head[b]; k >= 0; k = nxt[k]) {
            if (G1[k] == g1 && G2[k] == g2) { found = k; break; }
        }
        if (found < 0) {
            found = cnt++;
            G1[found] = g1;
            G2[found] = g2;
            nxt[found] = head[b];
            head[b] = found;
        }
        res[i] = (uint32_t)(found + 1);
    }
    free(spf);
    free(primes);
    free(G1);
    free(G2);
    free(head);
    free(nxt);
    if (rg_res) free(rg_res);
    rg_res = res;
    rg_N = N;
}

seq_t a(idx_t n) {
    if (n < 1) return 1;
    if (n > rg_N) {
        idx_t N = rg_N * 2;
        if (N < 1024) N = 1024;
        if (N < n) N = n;
        rg_build(N);
    }
    return (seq_t)rg_res[n];
}
