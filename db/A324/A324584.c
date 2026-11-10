/*
 * A324584  Numbers n that share a prime factor with A276086(n).
 * https://oeis.org/A324584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m such that gcd(m, A276086(m)) > 1; found by
 *            a search. gcd > 1 iff some prime p = prime(k+1) divides m and
 *            the k-th primorial-base digit of m, floor(m/primorial(k)) mod p,
 *            is nonzero
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 8920 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A324584_test(uint64_t m)
{
    static const uint64_t pr[15] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    uint64_t P = 1;
    for (int k = 0; k < 15; ++k) {
        if (P > m) break;
        uint64_t p = pr[k];
        if (m % p == 0 && ((m / P) % p) != 0) return true;
        P *= p;
    }
    return false;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t cur = 0;

    while (cnt < (size_t)n) {
        ++cur;
        if (A324584_test(cur)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
                if (!terms) abort();
            }
            terms[cnt++] = cur;
        }
    }
    return (seq_t)terms[(size_t)n - 1];
}
