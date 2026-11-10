/*
 * A368249  a(n) = A002378(A005117(n)-1).
 * https://oeis.org/A368249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m*(m+1) where m = A005117(n)-1 (A005117 = squarefree
 *            numbers; found by search)
 * Range:     u8 n <= 11, u16 n <= 157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 8198 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool A368249_sqfree(uint64_t k) {
    if (k % 4 == 0)
        return false;
    for (uint64_t i = 3; i * i <= k; i += 2) {
        if (k % (i * i) == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n) {
    static uint64_t *cache = 0;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    if (n < 1)
        return 0;

    if (n > cache_size) {
        if (n > cache_cap) {
            idx_t newcap = cache_cap ? cache_cap : 64;
            while (newcap < n)
                newcap *= 2;
            cache = (uint64_t *)realloc(cache, (size_t)newcap * sizeof(uint64_t));
            cache_cap = newcap;
        }
        uint64_t k = cache_size ? cache[cache_size - 1] : 0;
        for (idx_t i = cache_size; i < n; i++) {
            do {
                k++;
            } while (!A368249_sqfree(k));
            cache[i] = k;
        }
        cache_size = n;
    }

    seq_t m = (seq_t)(cache[n - 1] - 1);
    return m * (m + 1);
}
