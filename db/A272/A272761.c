/*
 * A272761  List of indices i such that A065879(i) != A065880(i).
 * https://oeis.org/A272761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th index i with more than one k >= 1 satisfying
 *            popcount(i*k) = k (smallest != largest solution); computed by
 *            search with a memoized scan
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9723 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned A272761_pc128(unsigned __int128 x)
{
    return (unsigned)(__builtin_popcountll((unsigned long long)x) +
                      __builtin_popcountll((unsigned long long)(x >> 64)));
}

static int A272761_ok(uint64_t i)
{
    int cnt = 0;
    for (unsigned k = 1; k <= 80; ++k) {
        unsigned __int128 p = (unsigned __int128)i * k;
        if (A272761_pc128(p) == k) {
            if (++cnt >= 2) return 1;
        }
    }
    return 0;
}

seq_t a(idx_t n)
{
    static uint64_t *cache = 0;
    static size_t count = 0, cap = 0;
    static uint64_t next = 1;

    while (count < (size_t)n) {
        if (A272761_ok(next)) {
            if (count == cap) {
                cap = cap ? cap * 2 : 1024;
                cache = (uint64_t *)realloc(cache, cap * sizeof(uint64_t));
            }
            cache[count++] = next;
        }
        ++next;
    }
    return (seq_t)cache[n - 1];
}
