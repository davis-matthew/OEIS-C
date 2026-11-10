/*
 * A319999  Filter sequence combining A173557(n) with A319993(n).
 * https://oeis.org/A319999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A173557(n), A319993(n)]; labels are assigned 1,2,3,... in order
 *            of first appearance of each pair. Computed by sieving all pairs
 *            up to n and hashing, cached across calls
 * Range:     u8 n <= 378, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 120 terms through n = 9981 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *rgs_label = NULL;
static uint64_t rgs_N = 0;

static void rgs_pair(uint64_t m, uint64_t *r, int64_t *s)
{
    uint64_t res = 1;
    int tz = 0;
    uint64_t t = m;
    while ((t & 1) == 0) { ++tz; t >>= 1; }
    if (tz > 0) res *= 1; /* p=2 contributes (2-1)=1 */
    uint64_t odd = t, rem = t, rad = 1;
    for (uint64_t p = 3; p <= rem / p; p += 2) {
        if (rem % p == 0) {
            res *= (p - 1);
            rad *= p;
            do { rem /= p; } while (rem % p == 0);
        }
    }
    if (rem > 1) { res *= (rem - 1); rad *= rem; }
    *r = res;
    if (tz > 1) *s = 0;
    else {
        int64_t v = (int64_t)(odd / rad);
        *s = (tz == 1) ? -v : v;
    }
}

static void rgs_build(uint64_t M)
{
    uint64_t *R = (uint64_t *)malloc((M + 1) * sizeof(uint64_t));
    int64_t *S = (int64_t *)malloc((M + 1) * sizeof(int64_t));
    uint64_t *L = (uint64_t *)malloc((M + 1) * sizeof(uint64_t));
    uint64_t cap = 1;
    while (cap < 2 * M + 2) cap <<= 1;
    uint64_t *tab = (uint64_t *)calloc(cap, sizeof(uint64_t)); /* stores index m (0 = empty) */
    uint64_t next = 0;
    for (uint64_t m = 1; m <= M; ++m) {
        rgs_pair(m, &R[m], &S[m]);
        uint64_t h = (R[m] * 0x9E3779B97F4A7C15ULL) ^ ((uint64_t)S[m] * 0xC2B2AE3D27D4EB4FULL);
        h ^= h >> 29;
        h *= 0xBF58476D1CE4E5B9ULL;
        h ^= h >> 32;
        uint64_t pos = h & (cap - 1);
        for (;;) {
            uint64_t k = tab[pos];
            if (k == 0) {
                tab[pos] = m;
                L[m] = ++next;
                break;
            }
            if (R[k] == R[m] && S[k] == S[m]) {
                L[m] = L[k];
                break;
            }
            pos = (pos + 1) & (cap - 1);
        }
    }
    free(R); free(S); free(tab);
    if (rgs_label) free(rgs_label);
    rgs_label = L;
    rgs_N = M;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if ((uint64_t)n > rgs_N) {
        uint64_t M = (uint64_t)n;
        if (M < 2 * rgs_N) M = 2 * rgs_N;
        if (M < 1024) M = 1024;
        rgs_build(M);
    }
    return (seq_t)rgs_label[(uint64_t)n];
}
