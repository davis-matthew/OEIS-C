/*
 * A367695  Numbers k such that k and k+1 are both exponentially odd numbers
 *          (A268335).
 * https://oeis.org/A367695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A367695 – Numbers k such that k and k+1 are both exponentially odd.
 * The implementation builds the sequence on demand, caching results between
 * calls.  An integer is exponentially odd iff every exponent in its prime
 * factorisation is odd.  For the range of indices typically requested
 * (up to 10 000) the values are well within 64‑bit, so the function uses
 * 64‑bit arithmetic internally and converts to the build‑dependent seq_t
 * type for the returned value.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
 * Prime table (dynamic simple sieve)
 * -------------------------------------------------------------------- */
static uint64_t *primes        = NULL;   /* list of primes */
static size_t    primes_cnt    = 0;      /* number of stored primes */
static size_t    primes_cap    = 0;      /* allocated capacity   */
static uint64_t  current_limit = 0;      /* maximal value sieved */

/* grow the prime array if needed */
static void
ensure_primes_capacity(size_t needed)
{
    if (needed <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    primes_cap = new_cap;
}

/* rebuild the prime list up to at least 'limit' */
static void
ensure_primes_upto(uint64_t limit)
{
    if (limit <= current_limit) return;

    /* simple sieve up to 'limit' */
    size_t sz = (size_t)(limit + 1);
    uint8_t *is_comp = (uint8_t *)calloc(sz, 1);     /* 0 = prime candidate */
    if (!is_comp) return;                            /* OOM fallback */

    size_t i;
    for (i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            size_t j;
            for (j = i * i; j <= limit; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    ensure_primes_capacity(limit / 10 + 10);  /* rough capacity */
    primes_cnt = 0;
    for (i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
        }
    }
    free(is_comp);
    current_limit = limit;
}

/* --------------------------------------------------------------------
 * Test for exponentially odd (all prime exponents are odd)
 * -------------------------------------------------------------------- */
static bool
is_exponentially_odd(uint64_t n)
{
    if (n <= 1) return true;   /* 1 is treated as exponentially odd */

    uint64_t tmp = n;
    size_t j;
    for (j = 0; j < primes_cnt; ++j) {
        uint64_t p = primes[j];
        if (p > tmp / p) break;          /* p*p > tmp */
        if (tmp % p == 0) {
            unsigned cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            if ((cnt & 1) == 0) return false;   /* even exponent */
        }
    }
    /* remaining factor > 1 is prime with exponent 1 (odd) */
    return true;
}

/* --------------------------------------------------------------------
 * Cached sequence array
 * -------------------------------------------------------------------- */
static seq_t *seq_arr   = NULL;   /* cached terms */
static size_t seq_len   = 0;      /* number of cached terms */
static size_t seq_cap   = 0;      /* allocated capacity   */
static uint64_t next_candidate = 1;   /* next integer to test */

/* ensure seq_arr has space for 'needed' elements */
static void
ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* --------------------------------------------------------------------
 * Main sequence function
 * -------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;                /* function undefined for n < 1 */

    ensure_seq_capacity((size_t)n);

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        /* Extend prime list to handle the current candidate */
        uint64_t limit = 1;
        while (limit * limit < next_candidate) ++limit;
        ensure_primes_upto(limit + 100); /* safety margin */

        while (!(is_exponentially_odd(next_candidate) &&
                 is_exponentially_odd(next_candidate + 1))) {
            ++next_candidate;
        }
        /* store found term */
        seq_arr[seq_len++] = (seq_t)next_candidate;
        ++next_candidate;                 /* next number to test */
    }

    return seq_arr[(size_t)n - 1];
}
