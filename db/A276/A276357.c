/*
 * A276357  Primes of the form (p*2^x-1)/3, where p is also prime and x is a
 *          positive integer.
 * https://oeis.org/A276357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A276357: primes a such that */
/* 3*a + 1 = p * 2^x with prime p (p may be 2) and x >= 1. */

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

seq_t a(idx_t n)
{
    /* Static cache for previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;
    static seq_t next_a = 3;       /* next odd candidate to test */

    /* Grow the cache on first use */
    if (cache == NULL) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Compute terms up to the requested index if necessary */
    while (cache_len < (size_t)n) {
        seq_t a_val = next_a;
        seq_t m = ((seq_t)3 * a_val) + 1;   /* 3*a + 1 */

        if ((m & ((seq_t)1)) == 0) {        /* require m even (x >= 1) */
            seq_t r = m;
            /* strip all factors of 2: r becomes the odd part of m */
            while ((r & ((seq_t)1)) == 0) {
                r >>= 1;
            }
            /* a must be prime */
            if (is_prime_big(a_val)) {
                /* accept if the odd part is prime or equals 1 (p = 2 case) */
                if (r == 1 || is_prime_big(r)) {
                    /* store the term in the cache */
                    if (cache_len == cache_cap) {     /* grow if necessary */
                        cache_cap *= 2;
                        cache = mem_resize(seq_t, cache, cache_cap);
                    }
                    cache[cache_len++] = a_val;
                }
            }
        }

        next_a += 2;  /* advance to the next odd number */
    }

    /* Return the cached term */
    return cache[n - 1];
}
