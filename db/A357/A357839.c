/*
 * A357839  a(n) is the greatest divisor > 1 of n which has already been
 *          listed, otherwise a(n) is the smallest number not yet listed; a(1)
 *          = 0.
 * https://oeis.org/A357839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357839:
 *  a(1) = 0.
 *  For n>1, a(n) is the greatest divisor d>1 of n that has already appeared
 *  earlier in the sequence.  If no such divisor exists, a(n) is the
 *  smallest positive integer not yet listed.
 *
 *  The implementation keeps a cache of computed terms and a boolean array
 *  'seen' marking which integers have appeared.  The routine works for
 *  any idx_t (uint64_t) and seq_t (unsigned integer or arbitrary‑precision
 *  integer).  It is thread‑unsafe but fast enough for n up to ~10 000. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"   /* gives seq_realloc */

static seq_t *cache = NULL;        /* a(1..cache_len) stored at cache[0..]    */
static size_t cache_len = 0;       /* number of terms stored                  */
static size_t cache_cap = 0;       /* allocated capacity of 'cache'           */

static bool *seen = NULL;          /* seen[x] == true iff x has appeared      */
static size_t seen_cap = 0;        /* allocated capacity of 'seen'            */
static idx_t next_missing = 1;     /* smallest positive integer not yet seen  */

/* Ensure the cache can hold at least 'cap' elements.  */
static void ensure_cache_capacity(size_t cap) {
    if (cap < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < cap) new_cap <<= 1;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Ensure the seen array can index up to 'cap' inclusive.  */
static void ensure_seen_capacity(idx_t cap) {
    if (cap < (idx_t)seen_cap) return;
    size_t new_cap = seen_cap ? seen_cap : 1;
    while (new_cap <= (size_t)cap) new_cap <<= 1;
    bool *tmp = (bool *)realloc(seen, new_cap * sizeof(bool));
    if (!tmp) exit(EXIT_FAILURE);
    /* initialize new entries to false */
    for (idx_t i = seen_cap; i < (idx_t)new_cap; ++i) tmp[i] = false;
    seen = tmp;
    seen_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* defensive for n<1 */

    /* Compute any missing terms up to index n */
    for (idx_t cur = cache_len + 1; cur <= n; ++cur) {
        seq_t val;

        if (cur == (idx_t)1) {          /* a(1) = 0 */
            val = 0;
        } else {
            seq_t best = 0;

            /* cur itself as divisor */
            if (cur < (idx_t)seen_cap && seen[cur]) best = cur;

            /* proper divisors */
            for (idx_t i = 2; i * i <= cur; ++i) {
                if (cur % i) continue;
                idx_t d1 = i;
                idx_t d2 = cur / i;
                if (d1 < (idx_t)seen_cap && seen[d1] && d1 > best) best = d1;
                if (d2 < (idx_t)seen_cap && seen[d2] && d2 > best) best = d2;
            }

            if (best) val = best;              /* divisor found */
            else       val = (seq_t)next_missing; /* smallest missing */
        }

        /* cache the result */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = val;

        /* mark the value as seen (if >0) */
        if (val > 0) {
            idx_t v = (idx_t)val;
            ensure_seen_capacity(v);
            seen[v] = true;
            if (v == next_missing) {
                ensure_seen_capacity(next_missing);
                while (next_missing < (idx_t)seen_cap && seen[next_missing])
                    ++next_missing;
            }
        }
    }

    return cache[n - 1];
}
