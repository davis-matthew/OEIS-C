/*
 * A280419  Primes in A132934.
 * https://oeis.org/A280419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, bref
 * Formula:   a(n) is the concatenation of 1 and the successive composite
 *            integers until the resulting number is prime
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Validated: partial, pass, 2 terms through n = 2 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/num_digits_idx.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_comp = 4;      /* next integer to examine for a composite */
static seq_t current      = 1;   /* value that will be extended with more composites */

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset‑1 sequence, undefined for n==0 */

    /* If the result was already computed, return it */
    if (n <= cache_len) return cache[n - 1];

    /* Allocate or grow the cache array */
    if (!cache) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
    }
    while (cache_len < n) {
        seq_t val = current;          /* start extending the current concatenation */

        while (1) {
            /* find the next composite number */
            while (is_prime(next_comp)) ++next_comp;

            /* append the composite to the current value */
            seq_t powd  = pow10(num_digits_idx(next_comp));
            val = val * powd + (seq_t)next_comp;
            ++next_comp;

            /* check if the new value is prime */
            if (is_prime_big(val)) {
                /* store the term in the cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();   /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
                current = val;          /* next term will continue from here */
                break;                  /* move on to compute the next term */
            }
        }
    }
    return cache[n - 1];
}
