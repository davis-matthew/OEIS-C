/*
 * A348198  Terms of A326835 having more divisors than any smaller term.
 * https://oeis.org/A348198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th term of A326835 (odd k whose divisors have distinct
 *            phi) with more divisors than every smaller term; found by a
 *            pruned DFS search over odd-prime factorizations
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

#define A348198_SLIM (1u << 22)

static unsigned char *A348198_comp = NULL;
static uint64_t A348198_L, A348198_D, A348198_best;

static void A348198_init(void)
{
    if (A348198_comp) return;
    A348198_comp = (unsigned char *)malloc(A348198_SLIM);
    if (!A348198_comp) exit(1);
    for (uint32_t i = 0; i < A348198_SLIM; ++i) A348198_comp[i] = 0;
    A348198_comp[0] = A348198_comp[1] = 1;
    for (uint64_t i = 2; i * i < A348198_SLIM; ++i)
        if (!A348198_comp[i])
            for (uint64_t j = i * i; j < A348198_SLIM; j += i) A348198_comp[j] = 1;
}

static bool A348198_isprime(uint64_t x)
{
    if (x < A348198_SLIM) return !A348198_comp[x];
    if ((x & 1) == 0) return false;
    for (uint64_t i = 3; i * i <= x; i += 2)
        if (!A348198_comp[i] && x % i == 0) return false;
    return true;
}

static uint64_t A348198_nextp(uint64_t p)
{
    p += 2;
    while (!A348198_isprime(p)) p += 2;
    return p;
}

static int A348198_cmp(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return x < y ? -1 : (x > y ? 1 : 0);
}

static void A348198_dfs(uint64_t t, uint64_t d, const uint64_t *ph, uint64_t start)
{
    for (uint64_t p = start;; p = A348198_nextp(p)) {
        uint64_t rem = A348198_L / t;
        if (p > rem) break;
        int K = 0;
        uint64_t q = 1;
        while (q * p <= rem) { q *= p; K++; }
        if (K > 40) K = 40;
        if ((d << K) <= A348198_D) break;
        uint64_t pe = 1;
        for (int e = 1;; ++e) {
            pe *= p;
            if (pe > A348198_L / t) break;
            size_t total = (size_t)d * (size_t)(e + 1);
            uint64_t *arr = (uint64_t *)malloc(total * sizeof(uint64_t));
            if (!arr) exit(1);
            size_t cnt = 0;
            for (size_t i = 0; i < (size_t)d; ++i) arr[cnt++] = ph[i];
            uint64_t cj = p - 1;
            for (int j = 1; j <= e; ++j) {
                for (size_t i = 0; i < (size_t)d; ++i) arr[cnt++] = ph[i] * cj;
                cj *= p;
            }
            qsort(arr, total, sizeof(uint64_t), A348198_cmp);
            bool ok = true;
            for (size_t i = 1; i < total; ++i)
                if (arr[i] == arr[i - 1]) { ok = false; break; }
            if (!ok) { free(arr); break; }
            uint64_t nd = d * (uint64_t)(e + 1);
            uint64_t nt = t * pe;
            if (nd > A348198_D) {
                if (A348198_best == 0 || nt < A348198_best) {
                    A348198_best = nt;
                    A348198_L = nt - 1;
                }
                free(arr);
                break;
            }
            A348198_dfs(nt, nd, arr, A348198_nextp(p));
            free(arr);
        }
    }
}

seq_t a(idx_t n)
{
    static uint64_t rec[128];
    static uint64_t recd[128];
    static int nrec = 0;
    if (n == 0 || n > 120) return 0;
    A348198_init();
    if (nrec == 0) { rec[0] = 1; recd[0] = 1; nrec = 1; }
    while ((idx_t)nrec < n) {
        uint64_t prev = rec[nrec - 1];
        uint64_t D = recd[nrec - 1];
        uint64_t L = prev * 4;
        for (;;) {
            A348198_L = L;
            A348198_D = D;
            A348198_best = 0;
            uint64_t root = 1;
            A348198_dfs(1, 1, &root, 3);
            if (A348198_best) break;
            L *= 4;
        }
        /* compute divisor count of best from its factorization */
        uint64_t m = A348198_best, dc = 1;
        for (uint64_t p = 3; p * p <= m; p += 2) {
            if (m % p == 0) {
                int e = 0;
                while (m % p == 0) { m /= p; e++; }
                dc *= (uint64_t)(e + 1);
            }
        }
        if (m > 1) dc *= 2;
        rec[nrec] = A348198_best;
        recd[nrec] = dc;
        nrec++;
    }
    return (seq_t)rec[n - 1];
}
