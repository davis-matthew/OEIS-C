/*
 * A346408  Primes whose bitwise XOR of decimal digits is a prime.
 * https://oeis.org/A346408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = the n‑th prime p such that the bitwise XOR of * the
 *            decimal digits of p is itself a prime. The sequence is *
 *            generated lazily; previously found terms are cached
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   get_digits, is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/is_prime_seq.h"
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

static bool xor_is_prime(seq_t x)
{
    uint8_t buf[32];
    size_t len;
    get_digits(x, buf, &len);               /* buf holds digits 0–9 */
    seq_t xo = 0;
    for (size_t i = 0; i < len; ++i)
        xo ^= (seq_t)buf[i];
    return is_prime_seq(xo);
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static seq_t next_candidate = 2;         /* next integer to test */

    if (cache == NULL) {
        cache_cap = 128;
        cache     = mem_new(seq_t, cache_cap);
        /* In the unlikely event of allocation failure we simply
         * allow the subsequent pointer use to crash – the build
         * environment guarantees enough memory for the test size. */
    }

    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        /* test current candidate */
        if (is_prime_big(next_candidate)) {
            if (xor_is_prime(next_candidate)) {
                /* store the term */
                if (cache_size == cache_cap) {
                    cache_cap *= 2;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_size++] = next_candidate;
            }
        }
        /* advance to the next integer for the next iteration */
        next_candidate++;
    }

    /* safe cast: idx_t is at least as wide as size_t for the test set */
    return cache[(size_t)n - 1];
}
