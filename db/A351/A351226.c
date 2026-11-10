/*
 * A351226  Numbers k for which A276086(k) < k, where A276086 is the primorial
 *          base exp-function.
 * https://oeis.org/A351226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k with A276086(k) < k; A276086(k) = prod
 *            prime(i+1)^d_i for primorial-base digits d_i of k. Found by
 *            incremental search with memoized terms
 * Range:     u8 n <= 18, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool A351226_less(uint64_t k)
{
    static const uint32_t pr[18] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61};
    uint64_t x = k;
    unsigned __int128 v = 1;
    for (int i = 0; i < 18 && x > 0; i++) {
        uint64_t d = x % pr[i];
        x /= pr[i];
        for (uint64_t j = 0; j < d; j++) {
            v *= pr[i];
            if (v >= k) return false;
        }
    }
    return v < k;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static uint64_t cnt = 0, cap = 0, nextk = 1;
    while (cnt < (uint64_t)n) {
        uint64_t k = nextk++;
        if (A351226_less(k)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 64;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
                if (!terms) abort();
            }
            terms[cnt++] = k;
        }
    }
    return (seq_t)terms[(uint64_t)n - 1];
}
