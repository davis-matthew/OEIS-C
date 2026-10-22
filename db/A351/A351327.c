/*
 * A351327  Numbers whose trajectory under iteration of the product of squares
 *          of nonzero digits map includes 1.
 * https://oeis.org/A351327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A351327: the nth smallest positive integer whose
   trajectory under the “product of squares of non‑zero digits” map eventually
   reaches 1.  The function performs a brute‑force search, extending a
   static cache of previously found terms. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* static cache of found terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_candidate = 1;

/* extend cache array as needed */
static void cache_push(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* test whether a value is in the sequence */
static bool is_term(seq_t start)
{
    seq_t v = start;
    /* path of values visited in this iteration */
    seq_t path[1024];
    size_t path_len = 0;

    while (true) {
        if (v == (seq_t)1) return true;

        /* check for a loop */
        for (size_t i = 0; i < path_len; ++i) {
            if (path[i] == v) return false;
        }
        path[path_len++] = v;

        /* compute next term: product of squares of non‑zero digits */
        seq_t prod = 1;
        seq_t x = v;
        while (x > 0) {
            unsigned d = (unsigned)(x % 10);
            if (d != 0) {
                seq_t sq = (seq_t)d * d;
                prod *= sq;
            }
            x /= 10;
        }
        v = prod;
    }
}

seq_t a(idx_t n)
{
    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        while (!is_term(next_candidate)) ++next_candidate;
        cache_push(next_candidate);
        ++next_candidate;
    }
    return cache[n - 1];
}
