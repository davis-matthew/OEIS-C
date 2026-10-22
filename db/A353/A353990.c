/*
 * A353990  a(1) = 1; for n > 1, a(n) is the smallest positive number that has
 *          not yet appeared that is coprime to a(n-1), does not equal
 *          a(n-1)+1, and whose binary expansion has no 1-bit in common with
 *          the binary expansion of a(n-1).
 * https://oeis.org/A353990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A353990
   a(1) = 1; for n > 1, a(n) is the smallest positive integer not yet used
   that satisfies:
       gcd(a(n-1), a(n)) = 1
       a(n) != a(n-1) + 1
       (a(n-1) & a(n)) == 0   (no common 1‑bits in binary)
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

typedef struct {
    seq_t *data;  /* array of computed terms */
    size_t len;   /* number of terms stored */
    size_t cap;   /* allocated capacity */
} seq_cache_t;

static seq_cache_t cache = { NULL, 0, 0 };

/* Ensure at least n slots are allocated */
static void ensure_capacity(size_t n) {
    if (cache.cap < n) {
        size_t new_cap = cache.cap ? cache.cap * 2 : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *new_data = (seq_t *)realloc(cache.data, new_cap * sizeof(seq_t));
        if (!new_data) abort();      /* allocation failed */
        cache.data = new_data;
        cache.cap  = new_cap;
    }
}

/* Linear search: true if val already appeared */
static bool used(seq_t val) {
    for (size_t i = 0; i < cache.len; ++i)
        if (cache.data[i] == val)
            return true;
    return false;
}

/* Generate terms up to index upTo (1‑based). */
static void generate(size_t upTo) {
    for (; cache.len < upTo; ++cache.len) {
        if (cache.len == 0) {            /* a(1) = 1 */
            cache.data[0] = (seq_t)1;
            continue;
        }
        seq_t prev = cache.data[cache.len - 1];
        seq_t cand = (seq_t)1;
        seq_t prev_plus_one = prev + (seq_t)1;
        while (true) {
            if (!used(cand) &&
                cand != prev_plus_one &&
                (prev & cand) == (seq_t)0 &&
                gcd(prev, cand) == (seq_t)1) {
                cache.data[cache.len] = cand;
                break;
            }
            ++cand;
        }
    }
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) abort(); /* sequence is 1‑based */

    /* handle overflow of idx_t to size_t */
    size_t target = (size_t)n;
    if (n > (idx_t)SIZE_MAX) abort();

    ensure_capacity(target);
    if (cache.len < target)
        generate(target);
    return cache.data[target - 1];
}
