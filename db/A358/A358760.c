/*
 * A358760  Numbers k for which A349905(k) is a multiple of 4, where
 *          A349905(k) is the arithmetic derivative applied to the prime
 *          shifted k.
 * https://oeis.org/A358760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A349905(k) == 0 (mod 4); found by
 *            incremental search, derivative mod 4 via prime factorization
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9790 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

static unsigned pw4_A358760(unsigned r, uint64_t e)
{
    unsigned res = 1;
    while (e--) res = (res * r) & 3;
    return res;
}

/* true if A003415(A003961(k)) is divisible by 4 */
static bool good_A358760(uint64_t k)
{
    if (k <= 1) return true;
    unsigned r[64];
    uint64_t ex[64];
    int c = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            r[c] = (unsigned)(next_prime(p) & 3);
            ex[c] = e;
            c++;
        }
    }
    if (m > 1) {
        r[c] = (unsigned)(next_prime(m) & 3);
        ex[c] = 1;
        c++;
    }
    unsigned sum = 0;
    for (int i = 0; i < c; i++) {
        unsigned t = (unsigned)(ex[i] & 3);
        t = (t * pw4_A358760(r[i], ex[i] - 1)) & 3;
        for (int j = 0; j < c; j++) {
            if (j != i) t = (t * pw4_A358760(r[j], ex[j])) & 3;
        }
        sum = (sum + t) & 3;
    }
    return sum == 0;
}

static uint64_t *terms_A358760 = NULL;
static uint64_t len_A358760 = 0, cap_A358760 = 0, next_A358760 = 1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (len_A358760 < n) {
        if (good_A358760(next_A358760)) {
            if (len_A358760 == cap_A358760) {
                uint64_t nc = cap_A358760 ? cap_A358760 * 2 : 1024;
                uint64_t *np = (uint64_t *)realloc(terms_A358760, nc * sizeof(uint64_t));
                if (!np) return (seq_t)0;
                terms_A358760 = np;
                cap_A358760 = nc;
            }
            terms_A358760[len_A358760++] = next_A358760;
        }
        next_A358760++;
    }
    return (seq_t)terms_A358760[n - 1];
}
