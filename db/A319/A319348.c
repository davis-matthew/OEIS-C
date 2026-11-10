/*
 * A319348  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A003557(i) = A003557(j) and A051953(i) = A051953(j), for all i,
 *          j >= 1.
 * https://oeis.org/A319348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pair
 *            [A003557(n), A051953(n)]; computed by scanning indices and
 *            numbering distinct pairs in order of first appearance (memoized)
 * Range:     u8 n <= 383, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 9873 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void A319348_pair(uint64_t n, uint64_t *u, uint64_t *c)
{
    uint64_t m = n, res = 1, phi = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            for (int i = 1; i < e; i++) res *= p;
            phi -= phi / p;
        }
    }
    if (m > 1) phi -= phi / m;
    *u = res;
    *c = n - phi;
}

static uint32_t *A319348_rgs = 0;
static uint64_t *A319348_d1 = 0, *A319348_d2 = 0;
static uint64_t A319348_done = 0, A319348_cap = 0, A319348_nd = 0;

seq_t a(idx_t n)
{
    if (n > A319348_done) {
        if (n + 1 > A319348_cap) {
            uint64_t nc = (uint64_t)n + 1;
            if (nc < 2 * A319348_cap) nc = 2 * A319348_cap;
            A319348_rgs = (uint32_t *)realloc(A319348_rgs, nc * sizeof(uint32_t));
            A319348_d1 = (uint64_t *)realloc(A319348_d1, nc * sizeof(uint64_t));
            A319348_d2 = (uint64_t *)realloc(A319348_d2, nc * sizeof(uint64_t));
            A319348_cap = nc;
        }
        for (uint64_t i = A319348_done + 1; i <= n; i++) {
            uint64_t u, c;
            A319348_pair(i, &u, &c);
            uint64_t j;
            for (j = 0; j < A319348_nd; j++)
                if (A319348_d1[j] == u && A319348_d2[j] == c) break;
            if (j == A319348_nd) {
                A319348_d1[j] = u;
                A319348_d2[j] = c;
                A319348_nd++;
            }
            A319348_rgs[i] = (uint32_t)(j + 1);
        }
        A319348_done = n;
    }
    return (seq_t)A319348_rgs[n];
}
