/*
 * A276462  Prime numbers that consist of k 2's digits followed by k+1 1's
 *          digits for some k >= 1.
 * https://oeis.org/A276462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A276462.
   Each term is the smallest prime whose decimal representation
   consists of k copies of the digit '2' followed by k+1 copies of
   the digit '1' for some k ≥ 1.
   The function memoises discovered primes so that calls in any
   order remain fast. */
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Cached primes: prime_cache[0] == a(1), prime_cache[1] == a(2), ... */
static seq_t *prime_cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
/* The value of k that was last examined. */
static idx_t last_k = 0;

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used; offset starts at 1 */

    /* Generate primes until we have the first n terms. */
    while (cache_len < n)
    {
        /* Next k to test. */
        ++last_k;

        /* 10^k and 10^(k+1). */
        seq_t pow_k  = pow10(last_k);
        seq_t pow_k1 = pow10(last_k + 1);

        /* (10^k - 1) / 9 and (10^(k+1)-1) / 9.  These are k‑digit and (k+1)-digit
           repunits of '1'. */
        seq_t Rk   = (pow_k - ((seq_t)1)) / ((seq_t)9);
        seq_t Rk1  = (pow_k1 - ((seq_t)1)) / ((seq_t)9);

        /* Candidate: 2·Rk followed by k+1 ones.  Build by
           2*Rk * 10^(k+1) + Rk1. */
        seq_t candidate = ((seq_t)2) * Rk * pow_k1 + Rk1;

        /* Test primality.  The provided utility implements an arbitrary-precision
           Miller‑Rabin or equivalent test. */
        if (is_prime_big(candidate))
        {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
                if (!tmp) exit(1);      /* Out of memory. */
                prime_cache = tmp;
                cache_cap = new_cap;
            }
            prime_cache[cache_len++] = candidate;
        }
    }

    /* Return the (n‑1)‑th cached prime. */
    return prime_cache[n - 1];
}
