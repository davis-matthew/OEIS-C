/*
 * A353565  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A353564(i) = A353564(j), where A353564(n) = Product_{d|n, d<n}
 *          A276086(phi(d)).
 * https://oeis.org/A353565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = restricted growth sequence transform of A353564, where
 *            A353564(n) = Product_{d|n, d<n} A276086(phi(d)); computed by a
 *            sieve, comparing exact prime-exponent vectors of A353564 (the
 *            primorial-base digits of phi(d), summed over proper divisors d)
 * Range:     u8 n <= 356, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 106 terms through n = 9762 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A353565_K 10

static uint32_t *A353565_rgs = NULL;
static idx_t A353565_N = 0;

static void A353565_build(idx_t N)
{
    static const uint64_t pr[A353565_K] = {2,3,5,7,11,13,17,19,23,29};
    size_t sz = (size_t)N + 1;
    uint32_t *phi = (uint32_t *)malloc(sz * sizeof(uint32_t));
    uint32_t *vec = (uint32_t *)calloc(sz * A353565_K, sizeof(uint32_t));
    uint32_t *rgs = (uint32_t *)malloc(sz * sizeof(uint32_t));
    size_t hs = 1;
    while (hs < 2 * sz) hs <<= 1;
    uint32_t *tab = (uint32_t *)calloc(hs, sizeof(uint32_t));
    if (!phi || !vec || !rgs || !tab) exit(1);

    for (size_t i = 0; i < sz; ++i) phi[i] = (uint32_t)i;
    for (size_t p = 2; p < sz; ++p)
        if (phi[p] == p)
            for (size_t m = p; m < sz; m += p)
                phi[m] -= phi[m] / (uint32_t)p;

    for (size_t d = 1; d < sz; ++d) {
        uint32_t dig[A353565_K];
        uint64_t x = phi[d];
        uint64_t P = 1;
        int k = 0;
        for (int i = 0; i < A353565_K; ++i) dig[i] = 0;
        while (x > 0 && k < A353565_K) {
            uint64_t dd = (x / P) % pr[k];
            dig[k] = (uint32_t)dd;
            x -= dd * P;
            P *= pr[k];
            ++k;
        }
        for (size_t m = 2 * d; m < sz; m += d) {
            uint32_t *v = vec + m * A353565_K;
            for (int i = 0; i < A353565_K; ++i) v[i] += dig[i];
        }
    }

    uint32_t next = 0;
    for (size_t m = 1; m < sz; ++m) {
        const uint32_t *v = vec + m * A353565_K;
        uint64_t h = 1469598103934665603ULL;
        for (int i = 0; i < A353565_K; ++i) {
            h ^= v[i];
            h *= 1099511628211ULL;
        }
        h ^= h >> 29;
        size_t pos = (size_t)(h & (hs - 1));
        for (;;) {
            uint32_t e = tab[pos];
            if (e == 0) {
                tab[pos] = (uint32_t)m;
                rgs[m] = ++next;
                break;
            }
            if (memcmp(vec + (size_t)e * A353565_K, v, A353565_K * sizeof(uint32_t)) == 0) {
                rgs[m] = rgs[e];
                break;
            }
            pos = (pos + 1) & (hs - 1);
        }
    }
    free(phi);
    free(vec);
    free(tab);
    if (A353565_rgs) free(A353565_rgs);
    A353565_rgs = rgs;
    A353565_N = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A353565_N) {
        idx_t target = A353565_N * 2;
        if (target < 1024) target = 1024;
        if (target < n) target = n;
        A353565_build(target);
    }
    return (seq_t)A353565_rgs[n];
}
