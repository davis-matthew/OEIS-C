/*
 * A331185  a(n) = n - prime(A067004(n)), where A067004 is the ordinal
 *          transform of number of divisors of n (A000005).
 * https://oeis.org/A331185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = n - A000040(A067004(n))
 * Range:     i8 n <= 143, i16 n <= 10201, i32 n <= 10201, i64 n <= 10201,
 *            i128 n <= 10201, big any n
 * Validated: standard, pass, 106 terms through n = 8506 (2026-11-03)
 * Helpers:   mem, prime_nth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* t(n) = A067004(n) is 1 + the number of k < n with d(k) = d(n); with the
   counts per divisor count kept, the terms are found in order and cached,
   a(n) = n - prime(t(n)). (The original filled fixed tables for n <= 10000
   and read past them.) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/prime_nth.h"
#include "../utils/mem.h"

static uint64_t tau_u64(uint64_t n)
{
    uint64_t d = 1;
    for (uint64_t p = 2; p * p <= n; ++p)
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            d *= e + 1;
        }
    return n > 1 ? 2 * d : d;
}

seq_t a(idx_t n)
{
    static seq_t *A = NULL;
    static idx_t len = 1, cap = 0;           /* A[1..len) known */
    static uint64_t *freq = NULL;            /* freq[d] = how many k < len have d(k) = d */
    static uint64_t fcap = 0;
    if (n == 0) return (seq_t)0;
    if (n + 1 > cap) {
        idx_t c = cap ? cap : 1024;
        while (c < n + 1) c *= 2;
        seq_t *t = mem_resize(seq_t, A, c);
        if (!t) abort();
        A = t;
        cap = c;
    }
    for (; len <= n; ++len) {
        uint64_t d = tau_u64(len);
        if (d >= fcap) {
            uint64_t c = fcap ? fcap : 64;
            while (c <= d) c *= 2;
            uint64_t *t = (uint64_t *)realloc(freq, c * sizeof *t);
            if (!t) abort();
            for (uint64_t i = fcap; i < c; ++i) t[i] = 0;
            freq = t;
            fcap = c;
        }
        uint64_t t = ++freq[d];
        A[len] = (seq_t)len - (seq_t)prime_nth(t);
    }
    return A[n];
}
