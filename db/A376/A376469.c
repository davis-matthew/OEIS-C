/*
 * A376469  Starts of runs of 3 consecutive integers in which each member of
 *          the run has at least one divisor of the form p^e with p <= e,
 *          where p is a prime.
 * https://oeis.org/A376469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A376469.
   a(n) is the start of the n-th run of three consecutive integers where
   each integer has a divisor of the form p^e with prime p and exponent e ≥ p.
   The method scans increasing natural numbers, marks numbers divisible by
   any prime power p^p (p ≤ e), and records each occurrence of three
   consecutive marked numbers.  The generated values are cached so that
   subsequent calls are very fast.  The implementation works for any
   upper bound that fits into seq_t and is efficient for n up to 10 000. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1000000U      /* size of each processing block */

/* cache for computed terms */
static seq_t *terms_cache = NULL;
static size_t  terms_len  = 0;
static size_t  terms_cap  = 0;

/* state for block scanning */
static uint64_t block_start = 2U;   /* first number of current block */
static bool     prev_prev    = false;   /* marker for (block_start-2) */
static bool     prev_curr    = false;   /* marker for (block_start-1) */

/* prime powers p^p for primes p with p^p <= 2^64 */
static const uint64_t PRIME_POWERS[] = {
    4ULL,                 /* 2^2   */
    27ULL,                /* 3^3   */
    3125ULL,              /* 5^5   */
    823543ULL,            /* 7^7   */
    285311670611ULL,      /* 11^11 */
    302875106592253ULL    /* 13^13 */
};
static const size_t NP = sizeof(PRIME_POWERS)/sizeof(PRIME_POWERS[0]);

/* add a term to the cache, growing the array if necessary */
static void add_term(seq_t val) {
    if (terms_len == terms_cap) {
        terms_cap = terms_cap? (terms_cap << 1) : 128;
        terms_cache = (seq_t *)realloc(terms_cache, terms_cap * sizeof(seq_t));
    }
    terms_cache[terms_len++] = val;
}

/* a(n) : nth term of the sequence */
seq_t a(idx_t n) {
    /* initialise cache on first call */
    if (terms_cache == NULL) {
        terms_cap    = 128;
        terms_cache  = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        block_start  = 2U;
        prev_prev     = prev_curr = false;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        uint64_t low  = block_start;
        uint64_t high = low + (uint64_t)BLOCK_SIZE - 1;

        /* marker array for current block */
        static unsigned char block[BLOCK_SIZE];
        memset(block, 0, BLOCK_SIZE);

        /* mark all numbers in the block that are divisible by any prime power */
        for (size_t i = 0; i < NP; ++i) {
            uint64_t pp   = PRIME_POWERS[i];
            uint64_t start = (low + pp - 1ULL) / pp * pp;
            if (start > high) continue;
            for (uint64_t m = start; m <= high; m += pp) {
                block[m - low] = 1;
            }
        }

        /* scan the block looking for triples of consecutive marked numbers */
        bool p2 = prev_prev;
        bool p1 = prev_curr;
        for (size_t idx = 0; idx < BLOCK_SIZE; ++idx) {
            bool cur = block[idx];
            if (p2 && p1 && cur) {
                uint64_t start_val = low + (uint64_t)idx - 2ULL;
                add_term((seq_t)start_val);
            }
            p2 = p1;
            p1 = cur;
        }

        /* prepare state for next block */
        if (BLOCK_SIZE >= 2) {
            prev_prev = block[BLOCK_SIZE - 2];
            prev_curr = block[BLOCK_SIZE - 1];
        } else {
            prev_prev = prev_curr;
            prev_curr = false;
        }

        block_start += BLOCK_SIZE;
    }

    return terms_cache[n - 1];
}
