/*
 * A337508  Primes such that neither the left half nor the right half of the
 *          prime is prime.
 * https://oeis.org/A337508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 965 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A337508: primes such that neither the left half nor the
   right half (ignoring the middle digit for odd-length numbers)
   is prime.  The function builds a cache of terms and expands it
   lazily until the requested index is reached. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Utility routines supplied by the build environment.  Only the
   ones actually used are included. */
#include "../utils/num_digits.h"
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

/* Cache of known terms */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next prime candidate to test.  11 is the first prime that can
   satisfy the conditions. */
static seq_t next_candidate = 11;

/* Append a new value to the cache, growing it if necessary. */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();           /* unrecoverable error */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main routine: return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but avoid UB */

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* A prime must be odd (except 2 which is excluded). */
        if (next_candidate == 2) {
            next_candidate = 3;
        }

        /* Test primality of the candidate. */
        if (is_prime_big(next_candidate)) {
            /* Length of the decimal representation. */
            int len = num_digits(next_candidate);
            int half = len / 2;

            seq_t left, right;

            if (len & 1) { /* odd number of digits – skip the middle digit */
                seq_t denom1 = pow10(half + 1);
                seq_t denom2 = pow10(half);
                left  = next_candidate / denom1;
                right = next_candidate % denom2;
            } else {        /* even number of digits */
                seq_t denom = pow10(half);
                left  = next_candidate / denom;
                right = next_candidate % denom;
            }

            /* Both halves must be non‑prime. */
            if (!is_prime_big(left) && !is_prime_big(right)) {
                cache_push(next_candidate);
            }
        }

        next_candidate++;
    }

    return cache[n - 1];
}
