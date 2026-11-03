/*
 * A270653  Integers k such that A003266(k) is divisible by k.
 * https://oeis.org/A270653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 241, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 7218 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS sequence A270653:
   integers k such that the product of the first k Fibonacci numbers
   is divisible by k.  The n-th term is returned by a(n).            */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if k divides the product F1*F2*...*Fk.  Uses only
   unsigned arithmetic; intermediate products are computed in 128 bits
   to avoid overflow.  */
static inline bool div_by_k(idx_t k)
{
    if (k == 0) return false;
    uint64_t mod = (uint64_t)k;
    uint64_t prod = 1 % mod;
    uint64_t f1 = 1 % mod, f2 = 1 % mod;
    for (idx_t i = 1; i <= k; ++i) {
        uint64_t fib;
        if (i <= 2) {
            fib = 1 % mod;
        } else {
            fib = (f1 + f2) % mod;
            f1 = f2;
            f2 = fib;
        }
        prod = (unsigned __int128)prod * fib % mod;
        if (prod == 0) break;
    }
    return prod == 0;
}

/* Static cache of found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;

/* Return the n-th element (n>=1) of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (cache_len < n) {
        if (div_by_k(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
