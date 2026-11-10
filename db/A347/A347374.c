/*
 * A347374  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A331410(i) = A331410(j) and A000593(i) = A000593(j), for all i,
 *          j >= 1.
 * https://oeis.org/A347374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A000593(n), A331410(n)]; computed by scanning k=1..n and
 *            numbering distinct pairs in order of first appearance (memoized)
 * Range:     u8 n <= 624, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 108 terms through n = 9697 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A347374_lpf(uint64_t n)
{
    uint64_t p = 1;
    while ((n & 1) == 0) { p = 2; n >>= 1; }
    for (uint64_t d = 3; d * d <= n; d += 2) {
        while (n % d == 0) { p = d; n /= d; }
    }
    if (n > 1) p = n;
    return p;
}

static uint64_t A347374_sigma_odd(uint64_t n)
{
    while ((n & 1) == 0) n >>= 1;
    uint64_t res = 1;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            uint64_t term = 1, pw = 1;
            while (n % d == 0) { n /= d; pw *= d; term += pw; }
            res *= term;
        }
    }
    if (n > 1) res *= (n + 1);
    return res;
}

static uint64_t A347374_steps(uint64_t n)
{
    uint64_t cnt = 0;
    while (n > 1 && (n & (n - 1)) != 0) {
        uint64_t p = A347374_lpf(n);
        n += n / p;
        ++cnt;
    }
    return cnt;
}

static uint32_t *A347374_ids = 0;
static uint64_t *A347374_pa = 0, *A347374_pb = 0;
static uint32_t *A347374_ht = 0;
static size_t A347374_cap = 0, A347374_done = 0, A347374_nd = 0, A347374_hcap = 0;

static size_t A347374_hash(uint64_t x, uint64_t y)
{
    uint64_t h = x * 0x9E3779B97F4A7C15ULL ^ (y + 0x7F4A7C15ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    h *= 0xBF58476D1CE4E5B9ULL;
    h ^= h >> 32;
    return (size_t)h;
}

static void A347374_rehash(size_t newcap)
{
    free(A347374_ht);
    A347374_ht = (uint32_t *)calloc(newcap, sizeof(uint32_t));
    A347374_hcap = newcap;
    for (size_t i = 0; i < A347374_nd; i++) {
        size_t h = A347374_hash(A347374_pa[i], A347374_pb[i]) & (newcap - 1);
        while (A347374_ht[h]) h = (h + 1) & (newcap - 1);
        A347374_ht[h] = (uint32_t)(i + 1);
    }
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N + 1 > A347374_cap) {
        size_t nc = A347374_cap * 2;
        if (nc < N + 1) nc = N + 1;
        if (nc < 1024) nc = 1024;
        A347374_ids = (uint32_t *)realloc(A347374_ids, nc * sizeof(uint32_t));
        A347374_pa = (uint64_t *)realloc(A347374_pa, nc * sizeof(uint64_t));
        A347374_pb = (uint64_t *)realloc(A347374_pb, nc * sizeof(uint64_t));
        A347374_cap = nc;
    }
    if (A347374_hcap == 0) A347374_rehash(2048);
    while (A347374_done < N) {
        size_t k = A347374_done + 1;
        uint64_t x = A347374_sigma_odd((uint64_t)k);
        uint64_t y = A347374_steps((uint64_t)k);
        if ((A347374_nd + 1) * 2 > A347374_hcap) A347374_rehash(A347374_hcap * 2);
        size_t h = A347374_hash(x, y) & (A347374_hcap - 1);
        uint32_t id = 0;
        while (A347374_ht[h]) {
            size_t j = A347374_ht[h] - 1;
            if (A347374_pa[j] == x && A347374_pb[j] == y) { id = (uint32_t)(j + 1); break; }
            h = (h + 1) & (A347374_hcap - 1);
        }
        if (!id) {
            A347374_pa[A347374_nd] = x;
            A347374_pb[A347374_nd] = y;
            A347374_nd++;
            id = (uint32_t)A347374_nd;
            A347374_ht[h] = id;
        }
        A347374_ids[k] = id;
        A347374_done = k;
    }
    return (seq_t)A347374_ids[N];
}
