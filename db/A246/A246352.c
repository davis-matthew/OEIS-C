/*
 * A246352  Numbers n such that A048673(n) >= n.
 * https://oeis.org/A246352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m such that A048673(m) >= m, where A048673(m)
 *            = (A003961(m)+1)/2; found by incremental search with cached
 *            terms
 * Range:     u8 n <= 138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9361 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t A246352_next_prime(uint64_t p)
{
    uint64_t q = p + 1;
    for (;; q++) {
        if (q < 2) continue;
        bool pr = true;
        for (uint64_t d = 2; d * d <= q; d++)
            if (q % d == 0) { pr = false; break; }
        if (pr) return q;
    }
}

static bool A246352_test(uint64_t m)
{
    unsigned __int128 prod = 1;
    uint64_t x = m;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            uint64_t np = A246352_next_prime(p);
            while (x % p == 0) { x /= p; prod *= np; }
        }
    }
    if (x > 1) prod *= A246352_next_prime(x);
    return (prod + 1) / 2 >= (unsigned __int128)m;
}

static uint64_t *A246352_cache = NULL;
static size_t A246352_cnt = 0, A246352_cap = 0;
static uint64_t A246352_last = 0;

seq_t a(idx_t n)
{
    while (A246352_cnt < (size_t)n) {
        A246352_last++;
        if (A246352_test(A246352_last)) {
            if (A246352_cnt == A246352_cap) {
                A246352_cap = A246352_cap ? A246352_cap * 2 : 1024;
                A246352_cache = (uint64_t *)realloc(A246352_cache, A246352_cap * sizeof(uint64_t));
            }
            A246352_cache[A246352_cnt++] = A246352_last;
        }
    }
    return (seq_t)A246352_cache[n - 1];
}
