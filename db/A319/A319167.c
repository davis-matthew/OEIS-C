/*
 * A319167  Primes p such that A194113(p) is a prime.
 * https://oeis.org/A319167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th prime p such that A194113(p) = Sum_{j=1..p}
 *            floor(j*sqrt(10)) is prime
 * Range:     u8 n <= 3, u16 n <= 309, u32 n <= 435, u64 n <= 435,
 *            u128 n <= 435, big any n
 * Validated: standard, pass, 59 terms through n = 429 (2026-11-08)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* floor(sqrt(x)) for 64-bit x, exact */
static uint64_t A319167_isqrt64(uint64_t x)
{
    uint64_t lo = 0, hi = 4294967295ULL;
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid * mid <= x) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

static uint64_t *A319167_found = 0;
static size_t A319167_cnt = 0, A319167_cap = 0;
static uint64_t A319167_j = 0;     /* last j processed */
static uint64_t A319167_sum = 0;   /* A194113(A319167_j) */

seq_t a(idx_t n)
{
    while ((idx_t)A319167_cnt < n) {
        uint64_t j = ++A319167_j;
        A319167_sum += A319167_isqrt64(10ULL * j * j);
        if (j >= 2 && is_prime_u64(j) && is_prime_u64(A319167_sum)) {
            if (A319167_cnt == A319167_cap) {
                size_t nc = A319167_cap ? 2 * A319167_cap : 256;
                uint64_t *p = (uint64_t *)malloc(nc * sizeof(uint64_t));
                size_t i;
                for (i = 0; i < A319167_cnt; ++i) p[i] = A319167_found[i];
                free(A319167_found);
                A319167_found = p;
                A319167_cap = nc;
            }
            A319167_found[A319167_cnt++] = j;
        }
    }
    return (seq_t)A319167_found[n - 1];
}
