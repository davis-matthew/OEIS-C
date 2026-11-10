/*
 * A350086  a(n) is the smallest totient number k > 1 such that A005277(n)*k
 *          is a nontotient number, or 0 if no such number exists.
 * https://oeis.org/A350086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest totient number k > 1 such that A005277(n)*k is a
 *            nontotient (0 if none); found by search, totient membership
 *            tested by inverse-phi DFS
 * Range:     u8 n <= 82, u16 n <= 8458, u32 n <= 8458, u64 n <= 8458,
 *            u128 n <= 8458, big any n
 * Validated: standard, pass, 93 terms through n = 7933 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A350086_nt_list = NULL;
static uint64_t A350086_nt_count = 0;
static uint64_t A350086_nt_bound = 0;

static void A350086_nt_build(uint64_t bound) {
    uint64_t lim = 8 * bound;
    uint32_t *phi = (uint32_t *)malloc((lim + 1) * sizeof(uint32_t));
    uint8_t *seen = (uint8_t *)calloc(bound + 1, 1);
    uint64_t i, j, k;
    for (i = 0; i <= lim; i++) phi[i] = (uint32_t)i;
    for (i = 2; i <= lim; i++) {
        if (phi[i] == i) {
            for (j = i; j <= lim; j += i) phi[j] -= phi[j] / (uint32_t)i;
        }
    }
    for (i = 1; i <= lim; i++) {
        if (phi[i] <= bound) seen[phi[i]] = 1;
    }
    free(phi);
    free(A350086_nt_list);
    A350086_nt_list = (uint32_t *)malloc((bound / 2 + 1) * sizeof(uint32_t));
    A350086_nt_count = 0;
    for (k = 2; k <= bound; k += 2) {
        if (!seen[k]) A350086_nt_list[A350086_nt_count++] = (uint32_t)k;
    }
    free(seen);
    A350086_nt_bound = bound;
}

static uint64_t A350086_A005277(uint64_t n) {
    uint64_t bound;
    bound = A350086_nt_bound ? A350086_nt_bound : 4096;
    while (A350086_nt_count < n) {
        if (A350086_nt_bound >= bound) bound *= 2;
        A350086_nt_build(bound);
    }
    return A350086_nt_list[n - 1];
}

static bool A350086_isprime(uint64_t n) {
    uint64_t i;
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0) return false;
    for (i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

static uint64_t *A350086_pr = NULL;
static size_t A350086_np = 0;

static bool A350086_dfs(uint64_t m, size_t idx) {
    size_t i;
    if (m == 1) return true;
    if (m & 1) return false;
    for (i = idx; i < A350086_np; i++) {
        uint64_t p = A350086_pr[i];
        uint64_t t;
        if (m % (p - 1) != 0) continue;
        t = m / (p - 1);
        for (;;) {
            if (A350086_dfs(t, i + 1)) return true;
            if (t % p == 0) t /= p; else break;
        }
    }
    return false;
}

static int A350086_cmp(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return x < y ? 1 : (x > y ? -1 : 0);
}

static bool A350086_is_totient(uint64_t m) {
    uint64_t *divs;
    size_t nd = 0, cap = 64, i;
    uint64_t d;
    bool res;
    if (m == 1) return true;
    if (m & 1) return false;
    divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            uint64_t e = m / d;
            if (nd + 2 > cap) {
                cap *= 2;
                divs = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
            }
            if (A350086_isprime(d + 1)) divs[nd++] = d + 1;
            if (e != d && A350086_isprime(e + 1)) divs[nd++] = e + 1;
        }
    }
    qsort(divs, nd, sizeof(uint64_t), A350086_cmp);
    A350086_pr = divs;
    A350086_np = nd;
    res = A350086_dfs(m, 0);
    A350086_pr = NULL;
    A350086_np = 0;
    free(divs);
    (void)i;
    return res;
}

seq_t a(idx_t n) {
    uint64_t r, k;
    if (n == 0) return (seq_t)0;
    r = A350086_A005277((uint64_t)n);
    for (k = 2; k < 2000000000ULL; k += 2) {
        if (!A350086_is_totient(k)) continue;
        if (!A350086_is_totient(r * k)) return (seq_t)k;
    }
    return (seq_t)0;
}
