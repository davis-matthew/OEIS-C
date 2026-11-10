/*
 * A355822  Numbers k such that A003961(k) and A276086(k) share a prime
 *          factor, where A003961 is fully multiplicative with a(p) =
 *          nextprime(p), and A276086 is primorial base exp-function.
 * https://oeis.org/A355822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that gcd(A003961(k), A276086(k)) > 1;
 *            computed by search comparing prime sets
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9752 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

static bool A355822_test(uint64_t k)
{
    uint64_t np[32];
    int cnt = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            np[cnt++] = next_prime(p);
        }
    }
    if (m > 1) np[cnt++] = next_prime(m);
    if (cnt == 0) return false;

    m = k;
    uint64_t q = 2;
    while (m > 0) {
        uint64_t d = m % q;
        m /= q;
        if (d) {
            for (int i = 0; i < cnt; i++)
                if (np[i] == q) return true;
        }
        q = next_prime(q);
    }
    return false;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t sz = 0, cap = 0;
    static uint64_t last = 0;

    while (sz < (size_t)n) {
        last++;
        if (A355822_test(last)) {
            if (sz == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
                if (!terms) abort();
            }
            terms[sz++] = last;
        }
    }
    return (seq_t)terms[(size_t)n - 1];
}
