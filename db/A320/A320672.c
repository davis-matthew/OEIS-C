/*
 * A320672  a(n) is the smallest divisor of (n+2)*(n+3) not yet in the
 *          sequence.
 * https://oeis.org/A320672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor of (n+2)*(n+3) not yet in the *
 *            sequence. Offset 0. The function is memoised; it works for any *
 *            order of calls. It keeps a dynamic sequence array and a bitset *
 *            that records all values that have already appeared
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_realloc.h"   /* seq_t* seq_realloc(seq_t*,size_t,size_t) */

/* ----------------------------------------------------------------- */
/* global state for the sequence                                       */
static seq_t *seq = NULL;          /* dynamic array of sequence terms   */
static size_t seq_len = 0;         /* number of computed terms          */
static size_t seq_cap = 0;         /* capacity of seq array             */

/* bitset to remember which numbers have already occurred; 1 bit per
 * integer, stored in 64‑bit words */
static uint64_t *used_bits = NULL;   /* array of 64‑bit words   */
static size_t used_bits_words = 0;   /* words currently allocated */

/* prime table for factorising products */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static uint64_t current_prime_limit = 0;  /* highest sieved prime      */

/* ----------------------------------------------------------------- */
/* ----  Utility functions  ---------------------------------------- */
/* Ensure that used_bits can address bit v.  Grow the array if needed. */
static void ensure_used_bits(uint64_t v)
{
    if (v == 0) return;              /* 0 is never used as a divisor */
    size_t needed = (v >> 6) + 1;    /* word index + 1 */
    if (needed <= used_bits_words) return;

    size_t new_words = used_bits_words ? used_bits_words * 2 : 64;
    while (new_words < needed) new_words <<= 1;

    used_bits = (uint64_t*)realloc(used_bits, new_words * sizeof(uint64_t));
    memset(used_bits + used_bits_words, 0,
           (new_words - used_bits_words) * sizeof(uint64_t));
    used_bits_words = new_words;
}

/* Mark the integer v as used in the sequence. */
static void mark_used(uint64_t v)
{
    ensure_used_bits(v);
    used_bits[v >> 6] |= 1ULL << (v & 63);
}

/* Test whether v has already appeared. */
static bool is_used(uint64_t v)
{
    if ((v >> 6) >= used_bits_words) return false;
    return (used_bits[v >> 6] >> (v & 63)) & 1ULL;
}

/* 64‑bit integer square root (floor).  Works for values up to 2^63-1. */
static uint64_t int_sqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = n, ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (mid == 0) { ans = 0; low = 1; continue; }
        if (mid <= n / mid) { ans = mid; low = mid + 1; }
        else                { high = mid - 1; }
    }
    return ans;
}

/* Generate all primes up to limit (inclusive), storing them in the
 * global array `primes`.  Uses a simple sieve of Eratosthenes. */
static void generate_primes_up_to(uint64_t limit)
{
    free(primes);
    primes = NULL;
    prime_cnt = 0;
    if (limit < 2) return;

    uint32_t sz = (uint32_t)limit + 1;
    uint8_t *is_comp = (uint8_t*)calloc(sz, 1);
    for (uint32_t p = 2; (uint64_t)p * p <= limit; ++p) {
        if (!is_comp[p]) {
            for (uint64_t j = (uint64_t)p * p; j <= limit; j += p)
                is_comp[(size_t)j] = 1;
        }
    }
    for (uint32_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {
            primes = (uint64_t*)realloc(primes,
                                         (prime_cnt + 1) * sizeof(uint64_t));
            primes[prime_cnt++] = p;
        }
    }
    free(is_comp);
    current_prime_limit = limit;
}

/* Ensure that all primes up to at least `limit` are available. */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= current_prime_limit) return;
    generate_primes_up_to(limit);
}

/* Factorise m, storing distinct prime factors in pf[] and their
 * exponents in exp[], and returning the count via *cnt_out. */
static void factor_uint64(uint64_t m, uint64_t *pf, int *exp, size_t *cnt_out)
{
    size_t cnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        if (m % primes[i] == 0) {
            int e = 0;
            while (m % primes[i] == 0) { m /= primes[i]; ++e; }
            pf[cnt] = primes[i];
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        pf[cnt] = m;
        exp[cnt] = 1;
        ++cnt;
    }
    *cnt_out = cnt;
}

/* Recursive search over all divisors of the product.  It keeps the
 * smallest unused divisor found in *best. */
static void find_min_unused(int idx, size_t total, const uint64_t *pf,
                            const int *ex, uint64_t cur, uint64_t *best)
{
    if (idx == total) {
        if (!is_used(cur) && cur < *best) *best = cur;
        return;
    }
    if (cur >= *best) return;          /* pruning */

    uint64_t val = cur;
    for (int e = 0; e <= ex[idx]; ++e) {
        find_min_unused(idx + 1, total, pf, ex, val, best);
        val *= pf[idx];
    }
}

/* Ensure seq has room for at least `needed` elements. */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 256;
    if (new_cap < needed) new_cap = needed;

    seq = seq_realloc(seq, seq_cap, new_cap);
    seq_cap = new_cap;
}

/* ----------------------------------------------------------------- */
/* Public function computing a(n) ---------------------------------- */
seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;  /* defensive for negative indices */

    /* generate terms up to n on demand */
    if ((size_t)n >= seq_len) {
        size_t old_len = seq_len;
        ensure_seq_cap((size_t)n + 1);

        for (size_t i = old_len; i <= (size_t)n; ++i) {
            /* product (i+2)*(i+3) fits in 64 bits for all practical n */
            uint64_t prod = (uint64_t)(i + 2) * (uint64_t)(i + 3);

            /* extend used_bits so that divisor indices are addressed */
            ensure_used_bits(prod);

            /* factorise prod */
            uint64_t pf[64];
            int     exp[64];
            size_t  pf_cnt;
            uint64_t sqrt_prod = int_sqrt_uint64(prod);
            ensure_primes_upto(sqrt_prod + 1);
            factor_uint64(prod, pf, exp, &pf_cnt);

            /* search for the smallest unused divisor */
            uint64_t best = prod + 1;   /* any actual divisor ≤ prod          */
            find_min_unused(0, pf_cnt, pf, exp, 1, &best);

            /* store result and mark it as used */
            seq[i] = (seq_t)best;
            mark_used(best);
        }
        seq_len = (size_t)n + 1;
    }

    return seq[(size_t)n];
}
