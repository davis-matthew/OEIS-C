/*
 * A351875  Positive difference of the distinct prime factors of A129521(n).
 * https://oeis.org/A351875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005382(n) - 1, where A005382 lists primes p with 2p-1
 *            also prime (found by search)
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A351875_cache = NULL;
static size_t A351875_len = 0;
static size_t A351875_cap = 0;
static uint64_t A351875_next = 2;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A351875_len < (size_t)n) {
        uint64_t p = A351875_next;
        A351875_next = (p == 2) ? 3 : p + 2;
        if (!is_prime_u64(p)) continue;
        if (!is_prime_u64(2 * p - 1)) continue;
        if (A351875_len == A351875_cap) {
            size_t nc = A351875_cap ? A351875_cap * 2 : 64;
            uint64_t *t = (uint64_t *)realloc(A351875_cache, nc * sizeof(uint64_t));
            if (!t) abort();
            A351875_cache = t;
            A351875_cap = nc;
        }
        A351875_cache[A351875_len++] = p;
    }
    return (seq_t)(A351875_cache[n - 1] - 1);
}
