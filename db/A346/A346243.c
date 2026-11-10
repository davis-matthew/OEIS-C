/*
 * A346243  Sum of A324198 and its Dirichlet inverse.
 * https://oeis.org/A346243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = A324198(n) + A346242(n), where A324198(n) = gcd(n,
 *            A276086(n)) and A346242 is its Dirichlet inverse: b(1)=1, b(n) =
 *            -Sum_{d|n, d<n} A324198(n/d)*b(d). Computed by a divisor sieve
 * Range:     i8 n <= 74, i16 n <= 6749, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 114 terms through n = 9573 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t A324198_val(uint64_t m)
{
    static const uint64_t P[15] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    uint64_t rem = m, result = 1;
    for (int i = 0; i < 15 && rem > 0; ++i) {
        uint64_t p = P[i];
        uint64_t digit = rem % p;
        rem /= p;
        if (digit == 0) continue;
        uint64_t t = m, v = 0;
        while (t % p == 0 && v < digit) { t /= p; ++v; }
        for (uint64_t k = 0; k < v; ++k) result *= p;
    }
    return result;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *A = mem_new(seq_t, N + 1);
    seq_t *b = mem_new(seq_t, N + 1);
    seq_t *s = mem_new(seq_t, N + 1);
    for (size_t m = 1; m <= N; ++m) A[m] = (seq_t)A324198_val((uint64_t)m);
    for (size_t m = 1; m <= N; ++m) {
        if (m == 1) b[m] = (seq_t)1;
        else b[m] = -s[m];
        for (size_t j = 2; m * j <= N; ++j) {
            s[m * j] += A[j] * b[m];
        }
    }
    seq_t res = A[N] + b[N];
    mem_free(A);
    mem_free(b);
    mem_free(s);
    return res;
}
