/*
 * A391412  Middle of 3 consecutive primes p1 < p2 < p3 such that the pattern
 *          of differences [p2-p1, p3-p2] does not occur earlier.
 * https://oeis.org/A391412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the middle prime of the nth triple of consecutive primes
 *              whose pair of gaps [p2-p1, p3-p2] is a new pattern.
 * Strategy: Generate primes in order, keep a hash table of seen patterns;
 *          each time a new pattern appears, store the middle prime.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"

static seq_t *seq_cache = NULL;          /* cached terms of the sequence   */
static size_t cache_len = 0;             /* number of terms stored         */
static size_t cache_cap = 0;             /* capacity of cache array         */

static uint64_t *pattern_tbl = NULL;     /* hash table for patterns       */
static size_t pattern_cap = 0;           /* capacity of pattern table      */
static size_t pattern_used = 0;          /* number of used entries         */
static const double LOAD_FACTOR = 0.7;   /* load factor threshold          */

static uint64_t prime1 = 2, prime2 = 3, prime3 = 5;  /* next triple to test   */
static bool initialized = false;

/* initialize hash table with given power‑of‑two capacity */
static void pattern_init(size_t capacity) {
    pattern_cap = capacity;
    pattern_tbl = (uint64_t *)calloc(pattern_cap, sizeof(uint64_t));
    pattern_used = 0;
}

/* simple open‑addressing insertion; return true if key was new   */
static bool pattern_insert(uint64_t key) {
    if (!pattern_tbl) pattern_init(8);
    if ((double)(pattern_used + 1) > pattern_cap * LOAD_FACTOR) {
        /* resize */
        size_t newcap = pattern_cap * 2;
        uint64_t *newtbl = (uint64_t *)calloc(newcap, sizeof(uint64_t));
        for (size_t i = 0; i < pattern_cap; ++i) {
            uint64_t cur = pattern_tbl[i];
            if (cur) {
                size_t idx = cur & (newcap - 1);
                while (newtbl[idx]) idx = (idx + 1) & (newcap - 1);
                newtbl[idx] = cur;
            }
        }
        free(pattern_tbl);
        pattern_tbl = newtbl;
        pattern_cap = newcap;
    }

    size_t idx = key & (pattern_cap - 1);
    while (1) {
        uint64_t cur = pattern_tbl[idx];
        if (!cur) {                      /* empty slot found */
            pattern_tbl[idx] = key;
            ++pattern_used;
            return true;                /* new pattern */
        }
        if (cur == key) return false;    /* already present */
        idx = (idx + 1) & (pattern_cap - 1);
    }
}

/* ensure cache has space for at least n elements            */
static void cache_ensure(size_t n) {
    if (cache_cap < n) {
        size_t newcap = cache_cap ? cache_cap * 2 : 64;
        while (newcap < n) newcap *= 2;
        seq_t *newarr = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
        if (!newarr) abort();           /* allocation failure */
        seq_cache = newarr;
        cache_cap = newcap;
    }
}

/* generate terms until cache_len == target */
static void generate_upto(idx_t target) {
    if (!initialized) {
        /* prepare initial state */
        cache_ensure(target);
        cache_len = 0;
        pattern_tbl = NULL;
        pattern_cap = 0;
        pattern_used = 0;
        prime1 = 2; prime2 = 3; prime3 = 5;
        initialized = true;
    }

    while ((idx_t)cache_len < target) {
        uint64_t g1 = prime2 - prime1;
        uint64_t g2 = prime3 - prime2;
        uint64_t key = (g1 << 32) | g2;
        if (pattern_insert(key)) {
            /* new pattern: store middle prime */
            cache_ensure(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)prime2;
        }
        /* advance to next triple */
        prime1 = prime2;
        prime2 = prime3;
        prime3 = next_prime(prime3);
    }
}

/* public function --------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, but harmless */
    generate_upto(n);
    return seq_cache[n - 1];
}
