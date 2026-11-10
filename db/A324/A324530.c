/*
 * A324530  Lexicographically earliest sequence such that a(i) = a(j) => f(i)
 *          = f(j), where f(n) = [A033879(n), A318458(n)] for all other
 *          numbers, except f(1) = -1.
 * https://oeis.org/A324530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank by first appearance of f(n)=[2n-sigma(n), n AND
 *            (sigma(n)-n)], f(1) unique; computed by sieve + hashing
 * Range:     u8 n <= 283, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 110 terms through n = 9467 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned *g_ids = NULL;
static uint64_t g_cap = 0;

static uint64_t hash_pair_324530(long long d, long long b)
{
    uint64_t h = (uint64_t)d * 0x9E3779B97F4A7C15ULL;
    h ^= (uint64_t)b * 0xC2B2AE3D27D4EB4FULL + 0x165667B19E3779F9ULL + (h << 6) + (h >> 2);
    h ^= h >> 29;
    h *= 0xBF58476D1CE4E5B9ULL;
    h ^= h >> 32;
    return h;
}

static void build_324530(uint64_t N)
{
    long long *sig = (long long *)calloc((size_t)N + 1, sizeof(long long));
    long long *dd = (long long *)malloc(((size_t)N + 1) * sizeof(long long));
    long long *bb = (long long *)malloc(((size_t)N + 1) * sizeof(long long));
    unsigned *ids = (unsigned *)calloc((size_t)N + 1, sizeof(unsigned));
    size_t tsz = 1;
    while (tsz < 2 * (size_t)N + 2) tsz <<= 1;
    uint64_t *tbl = (uint64_t *)calloc(tsz, sizeof(uint64_t));
    uint64_t i, j;
    unsigned next_id = 0;

    for (i = 1; i <= N; i++)
        for (j = i; j <= N; j += i)
            sig[j] += (long long)i;

    for (i = 1; i <= N; i++) {
        dd[i] = 2 * (long long)i - sig[i];
        bb[i] = (long long)i & (sig[i] - (long long)i);
    }

    ids[1] = ++next_id;
    for (i = 2; i <= N; i++) {
        size_t pos = (size_t)(hash_pair_324530(dd[i], bb[i]) & (tsz - 1));
        for (;;) {
            uint64_t f = tbl[pos];
            if (f == 0) {
                tbl[pos] = i;
                ids[i] = ++next_id;
                break;
            }
            if (dd[f] == dd[i] && bb[f] == bb[i]) {
                ids[i] = ids[f];
                break;
            }
            pos = (pos + 1) & (tsz - 1);
        }
    }

    free(sig);
    free(dd);
    free(bb);
    free(tbl);
    free(g_ids);
    g_ids = ids;
    g_cap = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if ((uint64_t)n > g_cap) {
        uint64_t N = g_cap * 2;
        if (N < (uint64_t)n) N = (uint64_t)n;
        if (N < 1024) N = 1024;
        build_324530(N);
    }
    return (seq_t)g_ids[n];
}
