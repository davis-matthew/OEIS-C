/*
 * A295886  Filter-sequence combining A003557(n) and A023900(n).
 * https://oeis.org/A295886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pair
 *            (A003557(n), A023900(n)); a(n) = rank of first occurrence of the
 *            pair, in order of first appearance. Computed by sieve
 * Range:     u8 n <= 297, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 109 terms through n = 8932 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A295886_val = 0;
static uint64_t A295886_cap = 0;

static uint64_t A295886_hash(uint64_t x, uint64_t y, uint64_t z) {
    uint64_t h = x * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    h += y * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 31;
    h += z * 0x165667B19E3779F9ULL;
    h ^= h >> 32;
    h *= 0xD6E8FEB86659FD93ULL;
    h ^= h >> 32;
    return h;
}

static void A295886_build(uint64_t N) {
    uint64_t *spf = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    uint64_t *ka = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    uint64_t *kp = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    uint64_t *kq = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    uint64_t tsz = 1;
    while (tsz < 2 * N + 2) tsz <<= 1;
    uint64_t *tab = (uint64_t *)calloc((size_t)tsz, sizeof(uint64_t));
    uint64_t *val = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    uint64_t i, j, next_id = 0;

    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    for (i = 1; i <= N; i++) {
        uint64_t m = i, rad = 1, phi = 1, par = 0;
        while (m > 1) {
            uint64_t p = spf[m];
            while (m % p == 0) m /= p;
            rad *= p;
            phi *= (p - 1);
            par ^= 1;
        }
        ka[i] = i / rad;
        kp[i] = phi;
        kq[i] = par;
        uint64_t h = A295886_hash(ka[i], kp[i], kq[i]) & (tsz - 1);
        for (;;) {
            uint64_t e = tab[h];
            if (e == 0) {
                tab[h] = i;
                val[i] = ++next_id;
                break;
            }
            if (ka[e] == ka[i] && kp[e] == kp[i] && kq[e] == kq[i]) {
                val[i] = val[e];
                break;
            }
            h = (h + 1) & (tsz - 1);
        }
    }
    free(spf); free(ka); free(kp); free(kq); free(tab);
    if (A295886_val) free(A295886_val);
    A295886_val = val;
    A295886_cap = N;
}

seq_t a(idx_t n) {
    if ((uint64_t)n > A295886_cap) {
        uint64_t N = (uint64_t)n;
        if (N < 2 * A295886_cap) N = 2 * A295886_cap;
        if (N < 1024) N = 1024;
        A295886_build(N);
    }
    return (seq_t)A295886_val[(uint64_t)n];
}
