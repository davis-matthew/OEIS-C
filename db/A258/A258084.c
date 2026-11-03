/*
 * A258084  Numbers k such that k concatenated with its reversal k' yields a
 *          prime when the rightmost digit of k and leftmost digit of k' are
 *          coalesced.
 * https://oeis.org/A258084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k where k is the n‑th integer (k ≥ 1) satisfying * (k/10)
 *            * 10^{digit_len(rev(k))} + rev(k) is an odd prime. * The
 *            rightmost digit of k is dropped and the reversed string of k *
 *            (with leading zeros removed) is appended; the resulting number
 *            is * a prime. For example, * k = 124 → rev(k) = 4...
 * Range:     u8 n <= 9, u16 n <= 524, u32 n <= 5493, u64 n <= 5493,
 *            u128 n <= 5493, big any n
 * Validated: standard, pass, 73 terms through n = 5386 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/num_digits.h"
#include "../utils/reverse_digits_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

static seq_t *cache = NULL;     /* cache of found sequence terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t last_k = 0;         /* last tested k value */

/* Ensure that 'cache' has room for at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    if (cache != NULL)
        free_seq_array(cache);

    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* safety for out‑of‑bounds calls */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        idx_t k = last_k + 1;   /* next candidate to test */

        for (;;) {
            idx_t rev_k = reverse_digits_idx(k);                     /* reversed k, with leading zeros removed */
            seq_t rev_seq = (seq_t)rev_k;
            int rev_digits = num_digits(rev_seq);      /* number of decimal digits of rev_k */

            seq_t first_part = (seq_t)(k / 10);       /* drop the rightmost digit of k */
            seq_t pow10_rev = pow10((idx_t)rev_digits);
            seq_t candidate = first_part * pow10_rev + rev_seq;

            if (is_prime_big(candidate)) {
                /* Found the next term.  Cache it and break. */
                last_k = k;
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = (seq_t)k;
                break;
            }
            ++k;   /* try next k */
        }
    }

    return cache[n - 1];
}
