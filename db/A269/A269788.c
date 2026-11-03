/*
 * A269788  Primes p such that 2*p + 47 is a square.
 * https://oeis.org/A269788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‐th prime p such that 2*p + 47 is a perfect square
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Compute the n-th term of A269788 */
seq_t a(idx_t n)
{
    /* sparse static cache: store found primes in increasing order */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* how many primes stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t  next_k     = 9;  /* odd k to test, starts at 9 */

    /* offset is 1; defensive handling of n == 0 */
    if (n == 0) return 0;

    /* initialise cache on first use */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return 0;               /* allocation failed → zero sentinel */
    }

    /* generate more terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* candidate prime: p = (k^2 - 47)/2   (k odd) */
        seq_t k  = (seq_t)next_k;
        seq_t prod = k * k;
        seq_t p  = (prod - (seq_t)47) / (seq_t)2;

        if (is_prime_big(p)) {                 /* use provided primality test */
            if (cache_len == cache_cap) {        /* grow capacity if needed */
                cache_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) {                    /* reallocation failed */
                    mem_free(cache);
                    cache = NULL;
                    cache_len = cache_cap = 0;
                    return 0;
                }
                cache = tmp;
            }
            cache[cache_len++] = p;
        }

        next_k += 2;      /* next odd integer */
    }

    return cache[n - 1];
}
