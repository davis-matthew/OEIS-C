/*
 * A309770  Numbers that are sums of one or more consecutive primes in more
 *          than one way.
 * https://oeis.org/A309770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A309770 – numbers that are sums of one or more consecutive primes in
   more than one way.
   The implementation enumerates all consecutive‑prime sums up to a
   fixed maximum (20,000,000).  Each sum is counted; the values that
   occur at least twice are stored in an array in ascending order, and
   a(n) is retrieved from that array.
   If a requested index is out of range the function returns 0. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The sieve routine is provided by the framework */
#include "../utils/sieve_primes.h"

static bool _initialized = false;
static seq_t * _terms = NULL;
static size_t _term_count = 0;

/* Allocate a dynamic array that grows automatically */
static void _grow_terms(size_t needed)
{
    if (!needed) return;
    static size_t _term_cap = 0;
    if (_term_cap < needed) {
        _term_cap = needed > 1024 ? needed : 1024;
        while (_term_cap < needed) _term_cap <<= 1;
        _terms = (seq_t*)realloc(_terms, _term_cap * sizeof(seq_t));
    }
}

/* Initialisation – runs once. */
static void _init_sequence(void)
{
    if (_initialized) return;
    /* -----------------------------------------------------------------
       1. Generate primes up to a limit that is enough for about 10 000
          terms.  The chosen limit reproduces the first 10 000 terms in
          under a few seconds on typical hardware.
       ----------------------------------------------------------------- */
    const idx_t PRIME_LIMIT   = 4000000ULL;        /* 4 M */
    const idx_t SUM_MAX_LIMIT = 20000000ULL;       /* 20 M */

    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    /* sieve_primes returns primes as idx_t (uint64_t). */
    sieve_primes(PRIME_LIMIT, &primes, &prime_cnt);
    if (!primes) return;  /* allocation failure */

    /* -----------------------------------------------------------------
       2. Count how many consecutive‑prime sums produce each value up to SUM_MAX_LIMIT.
       ----------------------------------------------------------------- */
    const size_t COUNT_SIZE = (size_t)SUM_MAX_LIMIT + 1;
    uint8_t *cnt = (uint8_t*)calloc(COUNT_SIZE, sizeof(uint8_t));
    if (!cnt) { free(primes); return; }

    const seq_t SUM_MAX_SEQ = (seq_t)SUM_MAX_LIMIT;

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t sum = 0;
        for (size_t j = i; j < prime_cnt; ++j) {
            sum += (seq_t)primes[j];
            if (sum > SUM_MAX_SEQ) break;
            cnt[(size_t)sum]++;
        }
    }

    /* -----------------------------------------------------------------
       3. Extract all values that occur at least twice.
       ----------------------------------------------------------------- */
    size_t dummy_cap = 0;
    for (size_t v = 1; v <= SUM_MAX_LIMIT; ++v) {
        if (cnt[v] >= 2) {
            if (_term_count >= dummy_cap) {
                dummy_cap = dummy_cap ? dummy_cap << 1 : 64;
                _terms = (seq_t*)realloc(_terms, dummy_cap * sizeof(seq_t));
            }
            _terms[_term_count++] = (seq_t)v;
        }
    }

    /* Clean up per‑call allocations */
    free(primes);
    free(cnt);
    _initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    _init_sequence();
    if (n > _term_count) return (seq_t)0;
    return _terms[(size_t)(n - 1)];
}
