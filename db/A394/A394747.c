/*
 * A394747  Numbers k such that k-4 and k+4 are prime and gcd(k, sigma(k)) is
 *          prime.
 * https://oeis.org/A394747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   gcd, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A394747:
   a(n) is the nth integer k (k ≥ 15) for which
      - k−4 and k+4 are prime,
      - gcd(k, σ(k)) is prime.
   The algorithm searches k = 3 + 6i (k ≡ 3 (mod 6)),
   testing the conditions and storing found values in a
   dynamically resized cache so that repeated calls are fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
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

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* current capacity of the array */

static idx_t next_k = 3;         /* next candidate k (k ≡ 3 (mod 6)) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 1, safeguard */

    /* Expand the cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t k = (seq_t)next_k;

        /* only candidates ≥ 9 have k‑4 ≥ 5; otherwise subtraction
           would underflow for unsigned types */
        if (next_k >= 9) {
            seq_t k_minus4 = k - (seq_t)4;
            seq_t k_plus4  = k + (seq_t)4;

            if (is_prime_big(k_minus4) && is_prime_big(k_plus4)) {
                seq_t sigma = sigma_of((idx_t)k);
                seq_t g     = gcd(k, sigma);
                if (is_prime_big(g)) {
                    /* Grow the cache if full */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                        if (!new_cache) abort();
                        cache = new_cache;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = k;
                }
            }
        }

        next_k += 6;   /* next candidate (always divisible by 3) */
    }

    return cache[n - 1];
}
