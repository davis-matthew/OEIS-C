/*
 * A326202  Lexicographically earliest sequence such that for all i, j, a(i) =
 *          a(j) => f(i) = f(j), where f(n) = 0 if n > 2 and gcd(n,sigma(n)) =
 *          1, with f(n) = n for all other numbers.
 * https://oeis.org/A326202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Lexicographically earliest sequence such that a(i) = a(j) ⇒ f(i)
 *            = f(j), where f(n) = 0 if n > 2 and gcd(n,σ(n)) = 1, f(n) = n
 *            otherwise
 * Range:     u8 n <= 443, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9633 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* f(n) = n is a different value for every n with f(n) != 0, so each such n
   is a class of its own, and all n with f(n) = 0 make one class. The
   lexicographically earliest sequence gives each new class the next unused
   value: a(n) is the value of the f = 0 class when f(n) = 0 and it already
   has one, else max so far + 1. Terms are cached (the original searched fixed
   tables of 50001 values, too few past n = 50000). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t sigma_u64(uint64_t n)
{
    uint64_t s = 0;
    for (uint64_t d = 1; d * d <= n; ++d)
        if (n % d == 0) {
            s += d;
            if (d != n / d) s += n / d;
        }
    return s;
}

static int f_is_zero(uint64_t n)
{
    if (n <= 2) return 0;
    uint64_t x = n, y = sigma_u64(n);
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x == 1;
}

seq_t a(idx_t n)
{
    static seq_t *A = NULL;
    static idx_t len = 1, cap = 0;           /* A[1..len) known */
    static seq_t maxv = 0, zero_val = 0;     /* zero_val = 0: the f = 0 class has no value yet */
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
        if (f_is_zero(len)) {
            if (zero_val == (seq_t)0) zero_val = maxv = maxv + (seq_t)1;
            A[len] = zero_val;
        } else {
            A[len] = maxv = maxv + (seq_t)1;
        }
    }
    return A[n];
}
