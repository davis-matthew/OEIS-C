/*
 * A258980  Primes in A258978.
 * https://oeis.org/A258980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A258978(A258979(n)), where A258978(m) = 1+s+s^2+s^3+s^4
 *            with s = sigma(m); a(n) is the n-th prime in A258978, found by
 *            scanning m = 1, 2, 3,
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 23, u64 n <= 932, u128 n <= 932,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 872 (2026-11-07)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static seq_t A258978_sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t A258978_a(idx_t n)
{
    seq_t s  = A258978_sigma_of(n);
    seq_t s2 = s * s;
    seq_t s3 = s2 * s;
    seq_t s4 = s3 * s;
    return (seq_t)1 + s + s2 + s3 + s4;
}

static idx_t *A258980_found = 0;   /* indices m where A258978(m) is prime */
static size_t A258980_cnt = 0;
static size_t A258980_cap = 0;
static idx_t A258980_next = 1;     /* next m to examine */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    while (A258980_cnt < (size_t)n) {
        idx_t m = A258980_next++;
        seq_t v = A258978_a(m);
        if (is_prime_seq(v)) {
            if (A258980_cnt == A258980_cap) {
                size_t nc = A258980_cap ? A258980_cap * 2 : 1024;
                idx_t *p = (idx_t *)realloc(A258980_found, nc * sizeof(idx_t));
                if (!p)
                    return (seq_t)0;
                A258980_found = p;
                A258980_cap = nc;
            }
            A258980_found[A258980_cnt++] = m;
        }
    }
    return A258978_a(A258980_found[(size_t)(n - 1)]);
}
