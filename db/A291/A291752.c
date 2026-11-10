/*
 * A291752  Compound filter: a(n) = P(A101296(n), A291751(n)), where P(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A291752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2 + (r+k)^2 - r - 3k)/2 with r = A101296(n) (rank of the
 *            prime signature of n among least representatives), k =
 *            A291751(n) (index of the first-occurrence class of the pair
 *            (A003557(n), A048250(n)))
 * Range:     u8 n <= 23, u16 n <= 503, u32 n <= 16385, u64 n <= 16385,
 *            u128 n <= 16385, big any n
 * Validated: standard, pass, 88 terms through n = 9700 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint64_t A291752_pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};

/* count products of primorials (nonincreasing exponents) <= L */
static uint64_t A291752_rec(int i, int maxe, uint64_t m, uint64_t L) {
    uint64_t total = 1;
    if (i >= 16) return total;
    uint64_t p = A291752_pr[i];
    for (int k = 1; k <= maxe; k++) {
        if (m > L / p) break;
        m *= p;
        total += A291752_rec(i + 1, k, m, L);
    }
    return total;
}

static uint64_t A291752_rank(uint64_t n) {
    int ex[24]; int len = 0;
    uint64_t x = n;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            int c = 0;
            while (x % p == 0) { x /= p; c++; }
            ex[len++] = c;
        }
    }
    if (x > 1) ex[len++] = 1;
    for (int i = 1; i < len; i++) {
        int key = ex[i]; int j = i;
        while (j > 0 && ex[j-1] < key) { ex[j] = ex[j-1]; j--; }
        ex[j] = key;
    }
    uint64_t L = 1;
    for (int i = 0; i < len; i++)
        for (int e = 0; e < ex[i]; e++) L *= A291752_pr[i];
    return A291752_rec(0, 64, 1, L);
}

static void A291752_pair(uint64_t m, uint64_t *u, uint64_t *s) {
    uint64_t r = 1, sg = 1, x = m;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            int c = 0;
            while (x % p == 0) { x /= p; c++; }
            for (int i = 1; i < c; i++) r *= p;
            sg *= (p + 1);
        }
    }
    if (x > 1) sg *= (x + 1);
    *u = r; *s = sg;
}

static uint64_t A291752_class(uint64_t n) {
    uint64_t tu, ts;
    A291752_pair(n, &tu, &ts);
    uint64_t cap = 16;
    while (cap < 2 * n + 2) cap <<= 1;
    uint64_t *k1 = (uint64_t*)calloc(cap, sizeof(uint64_t));
    uint64_t *k2 = (uint64_t*)calloc(cap, sizeof(uint64_t));
    uint64_t *id = (uint64_t*)calloc(cap, sizeof(uint64_t));
    if (!k1 || !k2 || !id) abort();
    uint64_t cnt = 0, result = 0;
    for (uint64_t m = 1; m <= n; m++) {
        uint64_t u, s;
        A291752_pair(m, &u, &s);
        uint64_t h = (u * 0x9E3779B97F4A7C15ULL) ^ (s * 0xC2B2AE3D27D4EB4FULL);
        h ^= h >> 29;
        uint64_t pos = h & (cap - 1);
        while (id[pos] && !(k1[pos] == u && k2[pos] == s)) pos = (pos + 1) & (cap - 1);
        if (!id[pos]) {
            cnt++;
            k1[pos] = u; k2[pos] = s; id[pos] = cnt;
        }
        if (u == tu && s == ts) { result = id[pos]; break; }
    }
    free(k1); free(k2); free(id);
    return result;
}

seq_t a(idx_t n) {
    seq_t r = (seq_t)A291752_rank((uint64_t)n);
    seq_t k = (seq_t)A291752_class((uint64_t)n);
    seq_t s = r + k;
    return (s * s + 2 - r - 3 * k) / 2;
}
