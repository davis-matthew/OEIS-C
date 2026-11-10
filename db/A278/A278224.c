/*
 * A278224  a(n) = A046523(A048673(n)).
 * https://oeis.org/A278224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(A048673(n)), where A048673(n) = (A003961(n)+1)/2
 *            and A046523(m) is the least number with the same prime signature
 *            as m
 * Range:     u8 n <= 47, u16 n <= 2429, u32 n <= 10500, u64 n <= 10500,
 *            u128 n <= 10500, big any n
 * Validated: standard, pass, 123 terms through n = 9559 (2026-11-08)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static seq_t A048673_a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]);
        for (uint64_t k = 0; k < exps[i]; ++k)
            prod = prod * (seq_t)np;
    }
    return (prod + (seq_t)1) / (seq_t)2;
}

seq_t a(idx_t n)
{
    seq_t m = A048673_a(n);
    int ex[200];
    int c = 0;
    seq_t d = 2;
    while (d * d <= m) {
        if (m % d == 0) {
            int e = 0;
            while (m % d == 0) {
                m = m / d;
                ++e;
            }
            ex[c++] = e;
        }
        d = d + (d == 2 ? 1 : 2);
    }
    if (m > 1)
        ex[c++] = 1;

    /* sort exponents descending (insertion sort) */
    for (int i = 1; i < c; ++i) {
        int v = ex[i];
        int j = i - 1;
        while (j >= 0 && ex[j] < v) {
            ex[j + 1] = ex[j];
            --j;
        }
        ex[j + 1] = v;
    }

    seq_t res = (seq_t)1;
    uint64_t p = 2;
    for (int i = 0; i < c; ++i) {
        for (int k = 0; k < ex[i]; ++k)
            res = res * (seq_t)p;
        p = next_prime(p);
    }
    return res;
}
