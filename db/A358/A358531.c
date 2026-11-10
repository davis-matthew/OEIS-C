/*
 * A358531  Indices of the primes in A358530.
 * https://oeis.org/A358531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A258026(n) + 2, i.e. the n-th k with prime(k)-prime(k-1)
 *            < prime(k-1)-prime(k-2); computed by a search over primes
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t *A358531_v = NULL;
static size_t A358531_cnt = 0, A358531_cap = 0;
static uint64_t A358531_p1 = 2, A358531_p0 = 3, A358531_k = 2; /* prime(k-1), prime(k), k */

static uint64_t A358531_next(uint64_t s)
{
    uint64_t x = s + 1;
    while (!is_prime((idx_t)x))
        ++x;
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    while (A358531_cnt < (size_t)n) {
        uint64_t q = A358531_next(A358531_p0);
        A358531_k++;
        if (q - A358531_p0 < A358531_p0 - A358531_p1) {
            if (A358531_cnt == A358531_cap) {
                size_t nc = A358531_cap ? A358531_cap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(A358531_v, nc * sizeof(uint64_t));
                if (!t)
                    return 0;
                A358531_v = t;
                A358531_cap = nc;
            }
            A358531_v[A358531_cnt++] = A358531_k;
        }
        A358531_p1 = A358531_p0;
        A358531_p0 = q;
    }
    return (seq_t)A358531_v[n - 1];
}
