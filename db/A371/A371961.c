/*
 * A371961  Nonnegative integers ordered by A371944 and then by value, with
 *          offset 0.
 * https://oeis.org/A371961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   A070939(a(n)) = A070939(n); within each bit-length block
 *            [2^(L-1), 2^L-1], a(n) is the k whose (A371944(k), k) has the
 *            same rank as n. Computed by sorting each block
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 88 terms through n = 8141 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef struct { uint64_t f; uint64_t k; } A371961_pair;

static int A371961_cmp(const void *x, const void *y)
{
    const A371961_pair *p = (const A371961_pair *)x;
    const A371961_pair *q = (const A371961_pair *)y;
    if (p->f != q->f) return p->f < q->f ? -1 : 1;
    if (p->k != q->k) return p->k < q->k ? -1 : 1;
    return 0;
}

static uint64_t A371961_f(uint64_t n)
{
    if (n == 0) return 0;
    int msb = 63;
    while (((n >> msb) & 1ULL) == 0) --msb;
    uint64_t result = 0;
    unsigned p0 = 0, p1 = 0;
    for (int i = msb; i >= 0; --i) {
        if ((n >> i) & 1ULL) { p1 ^= 1; result = (result << 1) | p1; }
        else                 { p0 ^= 1; result = (result << 1) | p0; }
    }
    return result;
}

seq_t a(idx_t n)
{
    static A371961_pair *cache = 0;
    static int cache_L = -1;

    if (n == 0) return (seq_t)0;

    int L = 0;
    uint64_t t = (uint64_t)n;
    while (t) { ++L; t >>= 1; }

    uint64_t lo = 1ULL << (L - 1);
    uint64_t size = lo;
    uint64_t r = (uint64_t)n - lo;

    if (cache_L != L) {
        free(cache);
        cache = (A371961_pair *)malloc((size_t)size * sizeof(A371961_pair));
        if (!cache) { cache_L = -1; return (seq_t)0; }
        for (uint64_t i = 0; i < size; ++i) {
            cache[i].k = lo + i;
            cache[i].f = A371961_f(lo + i);
        }
        qsort(cache, (size_t)size, sizeof(A371961_pair), A371961_cmp);
        cache_L = L;
    }
    return (seq_t)cache[r].k;
}
