/*
 * A392611  Numbers whose least significant nonzero digit is 1 in the
 *          primorial base representation (A049345), and that have also a
 *          digit larger than 1.
 * https://oeis.org/A392611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 152, u16 n <= 9892, u32 n <= 9892, u64 n <= 9892,
 *            u128 n <= 9892, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392611: Numbers whose least significant nonzero digit is 1 in the primorial
 * base representation (A049345), and that have at least one digit larger than 1.
 * Our implementation searches natural numbers sequentially, filtering by the
 * conditions above. */
 
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
 
/* Primorial base is derived from the list of primes.
 * We only need the first few primes because the product of the first 20 primes
 * is already enormous (>10^24), comfortably exceeding all values required for
 * n up to 10 000. */
static const unsigned long long _prime_list[20] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
    53ULL, 59ULL, 61ULL, 67ULL, 71ULL
};
 
/* Check whether a number n satisfies the definition.
 * We walk the primorial base representation from least significant
 * digit to most significant.  The first non‑zero digit must be 1,
 * and some higher digit must exceed 1. */
static bool _satisfies(seq_t n)
{
    if (n == 0) return false;
    seq_t rem = n;
    bool first_nonzero = false;
    bool has_big = false;
    size_t i = 0;
    while (rem > 0) {
        seq_t p = (seq_t)_prime_list[i];
        seq_t digit = rem % p;
        if (!first_nonzero) {
            if (digit != 0) {
                if (digit != 1) return false;   /* fails least‑significant‑nonzero‑digit test */
                first_nonzero = true;
            }
        }
        if (digit > 1) has_big = true;
        rem /= p;
        ++i;
        /* We never run out of the ten primes here for the problem limits. */
    }
    return first_nonzero && has_big;
}
 
/* Memoised generation of the sequence.  The array grows as needed. */
static seq_t * _cache = NULL;
static size_t   _cache_cap = 0;
static size_t   _cache_len = 0;
static seq_t   _next_candidate = 1;
 
static void _ensure_capacity(size_t needed)
{
    if (needed <= _cache_cap) return;
    size_t new_cap = _cache_cap ? _cache_cap * 2 : 8;
    if (needed > new_cap) new_cap = needed;
    _cache = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
    _cache_cap = new_cap;
}
 
seq_t a(idx_t n)
{
    /* Make sure we have at least n terms cached. */
    while (_cache_len < n) {
        /* Find next term that satisfies the conditions. */
        while (!_satisfies(_next_candidate)) ++_next_candidate;
        /* Store the term. */
        _ensure_capacity(_cache_len + 1);
        _cache[_cache_len++] = _next_candidate;
        ++_next_candidate;
    }
    return _cache[n - 1];
}
