/*
 * A296187  Yarborough primes that remain Yarborough primes when each of their
 *          digits are replaced by their squares.
 * https://oeis.org/A296187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation for sequence A296187: Find the n‑th
   Yarborough prime that remains a prime when each of its digits
   is replaced by that digit's square. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits.h"
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

/* Check whether x contains a digit 0 or 1. */
static inline bool has_zero_or_one(seq_t x)
{
    while (x > 0) {
        seq_t d = x % 10;
        if (d == 0 || d == 1)
            return true;
        x /= 10;
    }
    return false;
}

/* Replace each decimal digit of n by its square and concatenate. */
static seq_t transform_digits_to_squares(seq_t n)
{
    seq_t result = 0;
    seq_t temp   = n;
    int   len    = num_digits(temp);
    seq_t factor = pow10((idx_t)len - 1);

    while (temp > 0) {
        seq_t d   = temp / factor;      /* most significant digit */
        temp -= d * factor;
        factor /= 10;                   /* move to next digit */

        seq_t sq = d * d;               /* square of the digit */
        seq_t mul = (sq < 10) ? 10 : 100; /* digits of square added */
        result = result * mul + sq;
    }
    return result;
}

/* Persistent cache of discovered terms. */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t  next_test = 2;   /* integer to be examined next */

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* safe fallback for 0‑based calls */

    /* Extend cache until it contains the n‑th element. */
    while (cache_len < n) {
        seq_t cand = (seq_t)next_test;

        /* Candidate must have no 0 or 1 digits and be prime. */
        if (!has_zero_or_one(cand) && is_prime_big(cand)) {
            seq_t tr = transform_digits_to_squares(cand);

            /* Square‑digit transform must also be a Yarborough prime. */
            if (!has_zero_or_one(tr) && is_prime_big(tr)) {
                /* Grow the cache if necessary. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp)
                        abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cand;
            }
        }
        ++next_test;
    }
    return cache[n - 1];
}
