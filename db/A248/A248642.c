/*
 * A248642  Odious numbers (A000069) remaining odious if any digit is deleted
 *          (zeros allowed).
 * https://oeis.org/A248642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Odious numbers that remain odious after deleting any decimal digit. */
/* Algorithm: scan integers from 10 upwards, testing for odd popcount (odious) and
   that all single‑digit deletions also have odd popcount.  Results cached for
   arbitrary ordering of calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Powers of 10 up to 10^18 (enough for up to 19 decimal digits). */
static const uint64_t pow10[20] = {
    1ULL,10ULL,100ULL,1000ULL,10000ULL,100000ULL,1000000ULL,10000000ULL,
    100000000ULL,1000000000ULL,10000000000ULL,100000000000ULL,1000000000000ULL,
    10000000000000ULL,100000000000000ULL,1000000000000000ULL,
    10000000000000000ULL,100000000000000000ULL,1000000000000000000ULL,
    10000000000000000000ULL
};

/* Return true if x has an odd number of 1‑bits.  Works for integral types,
   unsigned __int128 and arbitrary precision integer classes. */
static inline bool is_odious(seq_t x)
{
    unsigned char parity = 0;
    while (x) {
        parity ^= 1;
        x = x & (x - 1);
    }
    return parity;
}

/* Cached sequence values. */
static seq_t *cache     = NULL;   /* array of found terms */
static size_t cache_sz  = 0;      /* how many terms stored */
static size_t cache_cap = 0;      /* allocated capacity */
static uint64_t next_candidate = 10;  /* next integer to test (>=10) */

/* Append a new term to the cache, growing the array as needed. */
static inline void add_term(seq_t val)
{
    if (cache_sz == cache_cap) {
        cache_cap = cache_cap == 0 ? 32 : cache_cap * 2;
        seq_t *newp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!newp) { /* allocation failed – abort */ abort(); }
        cache = newp;
    }
    cache[cache_sz++] = val;
}

/* Main sequence function.  Any number of calls with arbitrary order are
   supported because the cache keys are simply stored in insertion order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but safe guard */

    while (cache_sz < (size_t)n) {
        /* Test current candidate */
        uint64_t cand = next_candidate;
        seq_t cand_seq = (seq_t)cand;

        /* First check odiousness of the candidate itself */
        if (is_odious(cand_seq)) {
            /* Determine number of decimal digits */
            uint64_t temp = cand;
            unsigned int digits = 0;
            while (temp) { temp /= 10; ++digits; }
            if (digits >= 2) {
                bool ok = true;
                for (unsigned int i = 0; i < digits && ok; ++i) {
                    uint64_t high = cand / pow10[i + 1];
                    uint64_t low  = cand % pow10[i];
                    uint64_t newnum = high * pow10[i] + low;
                    if (!is_odious((seq_t)newnum)) ok = false;
                }
                if (ok) add_term(cand_seq);
            }
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
