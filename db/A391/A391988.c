/*
 * A391988  The number of exponential divisors of the exponentially squarefree
 *          numbers.
 * https://oeis.org/A391988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9856 (2026-11-03)
 * Helpers:   is_squarefree_i32, mem, num_divisors_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function computes the OEIS sequence A391988.
    a(n) = the number of exponential divisors of the n-th
    exponentially squarefree integer.  The implementation
    uses a simple sieve-like generation of the exponential
    squarefree numbers and a direct factorisation to count
    exponential divisors.  It works for any build where
    seq_t and idx_t are defined as required.  */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree_i32.h"
#include "../utils/num_divisors_i32.h"
#include "../utils/mem.h"

/* count exponential divisors of n:
   for each prime exponent e, multiply by τ(e) (number of divisors of e) */
static inline seq_t expdiv_count(uint64_t n) {
    seq_t cnt = 1;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            cnt *= num_divisors_i32(e);
        }
    }
    /* remaining prime factor exponent 1 contributes factor 1 */
    return cnt;
}

/* check whether n is exponentially squarefree:
   every prime exponent in n's factorisation is squarefree */
static inline bool is_exp_squarefree(uint64_t n) {
    if (n == 1) return true;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            if (!is_squarefree_i32(e)) return false;
        }
    }
    /* remaining prime factor has exponent 1 – OK */
    return true;
}

static inline bool is_prime_of_small(int x) { return x>1 && (x==2||x==3||x==5||x==7||x==11||x==13||x==17||x==19||x==23||x==29||x==31); }

/* the cache for computed sequence values and the corresponding
   n-th exponentially squarefree numbers */
static idx_t cached_max = 0;
static seq_t *cache_a = NULL;
static uint64_t *cache_m = NULL;

/* grow the cache arrays to hold at least |n| entries (1‑based) */
static void ensure_cache_size(idx_t n) {
    if (n <= cached_max) return;
    idx_t new_cap = n + 1; /* index 0 unused */
    cache_a = mem_resize(seq_t, cache_a, new_cap);
    cache_m = (uint64_t *)realloc(cache_m, new_cap * sizeof(uint64_t));
    /* compute from previous max +1 up to n */
    uint64_t current = (cached_max == 0) ? 1 : cache_m[cached_max] + 1;
    for (idx_t idx = cached_max + 1; idx <= n; ++idx) {
        /* find next exponent‑squarefree number */
        while (!is_exp_squarefree(current)) ++current;
        cache_m[idx] = current;
        cache_a[idx] = expdiv_count(current);
        ++current;
    }
    cached_max = n;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* undefined for n<1, return 0 for safety */
    ensure_cache_size(n);
    return cache_a[n];
}
