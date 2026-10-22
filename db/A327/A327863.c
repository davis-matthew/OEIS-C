/*
 * A327863  Numbers whose arithmetic derivative is a multiple of 3, cf.
 *          A003415.
 * https://oeis.org/A327863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A327863 – Numbers whose arithmetic derivative is a multiple of 3.
   A term is in the sequence iff
   • the number is a multiple of 9,  or
   • the number is not divisible by 3 and the sum of its prime factors
     (with repetition) is ≡ 0 (mod 3).  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Smallest‑prime‑factor helper using trial division. */
static uint64_t _spf(uint64_t n)
{
    if (n < 2) return n;
    if (n % 2ULL == 0ULL) return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) return d;
    }
    return n;  /* n is prime */
}

/* Test whether m is in the sequence. */
static bool in_seq(uint64_t m)
{
    if (m % 9ULL == 0ULL) return true;
    if (m % 3ULL == 0ULL) return false;

    uint64_t n = m;
    unsigned sum_mod3 = 0U;

    while (n > 1ULL) {
        uint64_t p = _spf(n);
        while (n % p == 0ULL) {
            sum_mod3 += (unsigned)(p % 3ULL);
            sum_mod3 %= 3U;
            n /= p;
        }
    }
    return sum_mod3 == 0U;
}

/* Dynamic storage for the produced terms. */
static seq_t *seq = NULL;
static size_t seq_len = 0;   /* number of cached terms */
static size_t seq_cap = 0;

/* Grow the cache to hold at least needed elements. */
static void ensure_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Return the n‑th term (1‑based).  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */
    if (seq == NULL) {                     /* first call: initialise */
        ensure_cap(128);
        seq[0] = (seq_t)0;                 /* a(1) = 0 */
        seq_len = 1;
    }
    uint64_t last = (uint64_t)seq[seq_len - 1];

    /* Produce sequence values until we have at least n terms. */
    while (seq_len < (size_t)n) {
        /* Find the next candidate. */
        uint64_t cand = last + 1ULL;
        while (!in_seq(cand)) ++cand;
        ensure_cap(seq_len + 1);
        seq[seq_len++] = (seq_t)cand;
        last = cand;
    }
    return seq[n - 1];
}
