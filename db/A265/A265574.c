/*
 * A265574  LCM-transform of triangular numbers.
 * https://oeis.org/A265574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of prime powers in t_n that were not present in
 *            any earlier t_k with k < n, where t_n = n*(n+1)/2. In other
 *            words, a(n) = t_n / gcd(t_n, LCM(t_1,…,t_{n-1})). a(1) = 1; all
 *            other terms follow from the recurrence above
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Validated: partial, pass, 97 terms through n = 97 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The LCM-transform: a(k) = L(k) / L(k-1) with L(k) = lcm of T(1..k),
   T(k) = k(k+1)/2; i.e. the product of p^(e - m_p) over the prime powers
   p^e || T(k) with e above the largest exponent m_p of p so far. The
   exponents are kept per prime value (all primes of T(k) are <= k + 1) in
   an array that grows with k, and the terms are cached. (The original passed
   the new capacity as the old one to its realloc, copying past the old
   array, and had prime tables only up to 10001.) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *A = NULL;
    static idx_t len = 1, cap = 0;           /* A[1..len) known */
    static unsigned char *mx = NULL;         /* mx[p] = largest exponent of p so far */
    static size_t mcap = 0;
    if (n == 0) return (seq_t)0;
    if (n + 1 > cap) {
        idx_t c = cap ? cap : 64;
        while (c < n + 1) c *= 2;
        seq_t *t = mem_resize(seq_t, A, c);
        if (!t) abort();
        A = t;
        cap = c;
    }
    if ((size_t)n + 2 > mcap) {
        size_t c = mcap ? mcap : 64;
        while (c < (size_t)n + 2) c *= 2;
        unsigned char *t = (unsigned char *)realloc(mx, c);
        if (!t) abort();
        for (size_t i = mcap; i < c; ++i) t[i] = 0;
        mx = t;
        mcap = c;
    }
    for (; len <= n; ++len) {
        uint64_t k = len, m = k * (k + 1) / 2;
        seq_t v = (seq_t)1;
        for (uint64_t p = 2; p * p <= m; ++p) {
            if (m % p) continue;
            unsigned e = 0;
            while (m % p == 0) { m /= p; ++e; }
            for (unsigned j = mx[p]; j < e; ++j) v = v * (seq_t)p;
            if (e > mx[p]) mx[p] = (unsigned char)e;
        }
        if (m > 1 && mx[m] < 1) { v = v * (seq_t)m; mx[m] = 1; }
        A[len] = v;
    }
    return A[n];
}
