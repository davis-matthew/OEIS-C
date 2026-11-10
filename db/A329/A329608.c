/*
 * A329608  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), where f(n) = A329605(n) for all other n, except
 *          for odd primes p, f(p) = 0.
 * https://oeis.org/A329608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of first occurrence of f(n) among f(1..n)
 *            (restricted-growth transform), where f(p) = 0 for odd primes p
 *            and f(n) = A329605(n) otherwise; A329605(n) =
 *            Product_{k=1..pi(gpf(n))} (1 + Sum_{i>=k} e_i), with n = Product
 *            prime(i)^e_i. f values are compared exactly via their
 *            prime-exponent...
 * Range:     u8 n <= 513, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9876 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define A329608_NP 18
#define A329608_W  (A329608_NP + 1)

static const uint32_t a329608_sp[A329608_NP] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61
};

static uint32_t *g_ids = NULL;
static idx_t g_cap = 0;

static uint64_t a329608_hash(const uint32_t *v)
{
    uint64_t h = 1469598103934665603ull;
    for (int i = 0; i < A329608_W; i++) {
        h ^= (uint64_t)v[i];
        h *= 1099511628211ull;
        h ^= h >> 29;
    }
    h *= 11400714819323198485ull;
    return h ^ (h >> 31);
}

static void a329608_build(idx_t N)
{
    size_t n = (size_t)N;
    uint32_t *spf = (uint32_t *)calloc(n + 1, sizeof(uint32_t));
    uint32_t *pi = (uint32_t *)calloc(n + 1, sizeof(uint32_t));
    uint32_t *ids = (uint32_t *)calloc(n + 1, sizeof(uint32_t));
    uint32_t *vecs = (uint32_t *)calloc((n + 2) * A329608_W, sizeof(uint32_t));
    if (!spf || !pi || !ids || !vecs) exit(1);
    uint32_t cnt = 0;
    for (size_t i = 2; i <= n; i++) {
        if (spf[i] == 0) {
            cnt++;
            for (size_t j = i; j <= n; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
        pi[i] = cnt;
    }
    /* factorization exponents of small numbers k = 1..64 over a329608_sp */
    uint32_t fe[65][A329608_NP];
    memset(fe, 0, sizeof(fe));
    for (uint32_t k = 2; k <= 64; k++) {
        uint32_t r = k;
        for (int s = 0; s < A329608_NP; s++)
            while (r % a329608_sp[s] == 0) { r /= a329608_sp[s]; fe[k][s]++; }
    }

    size_t H = 1;
    while (H < 2 * n + 2) H <<= 1;
    int *head = (int *)malloc(H * sizeof(int));
    int *nxt = (int *)malloc((n + 2) * sizeof(int));
    if (!head || !nxt) exit(1);
    for (size_t i = 0; i < H; i++) head[i] = -1;
    uint32_t distinct = 0;

    for (size_t m = 1; m <= n; m++) {
        uint32_t v[A329608_W];
        memset(v, 0, sizeof(v));
        if (m > 1 && (m & 1) && spf[m] == m) {
            v[A329608_NP] = 1;            /* marker for f = 0 */
        } else if (m > 1) {
            uint32_t ps[64], es[64];
            int c = 0;
            size_t r = m;
            while (r > 1) {
                uint32_t p = spf[r];
                uint32_t e = 0;
                while (r % p == 0) { r /= p; e++; }
                ps[c] = p; es[c] = e; c++;
            }
            uint32_t E = 0;
            for (int j = c - 1; j >= 0; j--) {
                E += es[j];
                uint32_t ij = pi[ps[j]];
                uint32_t inext = (j > 0) ? pi[ps[j - 1]] : 0;
                uint32_t count = ij - inext;
                uint32_t q = E + 1;       /* q <= 64 */
                for (int s = 0; s < A329608_NP; s++)
                    v[s] += fe[q][s] * count;
            }
        }
        size_t h = (size_t)(a329608_hash(v) & (uint64_t)(H - 1));
        int found = -1;
        for (int k = head[h]; k != -1; k = nxt[k]) {
            if (memcmp(vecs + (size_t)k * A329608_W, v, sizeof(v)) == 0) { found = k; break; }
        }
        if (found < 0) {
            memcpy(vecs + (size_t)distinct * A329608_W, v, sizeof(v));
            nxt[distinct] = head[h];
            head[h] = (int)distinct;
            found = (int)distinct;
            distinct++;
        }
        ids[m] = (uint32_t)found + 1;
    }
    free(vecs);
    free(head);
    free(nxt);
    free(spf);
    free(pi);
    free(g_ids);
    g_ids = ids;
    g_cap = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;
    if (n > g_cap) {
        idx_t N = 2 * g_cap;
        if (N < n) N = n;
        if (N < 1024) N = 1024;
        a329608_build(N);
    }
    return (seq_t)g_ids[(size_t)n];
}
