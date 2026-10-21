/*
 * A299971  Lexicographic first sequence of positive integers such that a(n) +
 *          a(n+1) has a digit 0, and no term occurs twice.
 * https://oeis.org/A299971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   digit_frequencies, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy selection: a(1)=1; for n>1 choose the smallest unused
 * positive integer x such that a(n‑1)+x contains a digit 0.
 *
 * The implementation caches already computed terms and a bitmap of
 * used values.  It works with any seq_t type, including arbitrary‑
 * precision integers, by using the seq_alloc/seq_realloc helpers.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/digit_frequencies.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* cached sequence terms          */
static size_t cache_len = 0;         /* number of terms cached (size_t) */

static unsigned char *used = NULL;   /* bitmap of used positive integers */
static size_t used_cap = 0;          /* current capacity of 'used' array */

/* Ensure the 'used' bitmap can index array element x (1‑based). */
static void ensure_used(idx_t x)
{
    if ((size_t)x + 1 <= used_cap)
        return;

    size_t new_cap = used_cap ? used_cap << 1 : 1;
    while (new_cap <= (size_t)x + 1)
        new_cap <<= 1;

    unsigned char *new_used = (unsigned char *)realloc(used,
                                  new_cap * sizeof(unsigned char));
    if (!new_used && new_cap > 0)
        abort();                       /* allocation failure */

    /* Initialise added part to 0. */
    for (size_t i = used_cap; i < new_cap; ++i)
        new_used[i] = 0;

    used = new_used;
    used_cap = new_cap;
}

/* Return true if decimal representation of num contains digit 0. */
static bool has_zero_digit(seq_t num)
{
    unsigned int freq[10] = {0};
    digit_frequencies(num, freq);
    return freq[0] != 0;
}

/* Generate cache up to (and including) index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Grow cache array if necessary. */
    size_t new_cap = cache_len ? cache_len : 1;
    while (new_cap < (size_t)n)
        new_cap <<= 1;

    if (cache == NULL) {
        cache = seq_alloc(new_cap);
    } else {
        cache = seq_realloc(cache, cache_len, new_cap);
    }

    /* Compute terms starting from the current length. */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t next_val;
        if (i == 0) {
            /* First term fixed to 1. */
            next_val = 1;
            cache[0] = next_val;
            ensure_used((idx_t)next_val);
            used[(idx_t)next_val] = 1;
            continue;
        }

        seq_t prev = cache[i - 1];
        idx_t candidate = 1;

        for (;;) {
            if ((size_t)candidate + 1 > used_cap)
                ensure_used(candidate);

            if (!used[(idx_t)candidate]) {
                seq_t sum = prev + (seq_t)candidate;
                if (has_zero_digit(sum)) {
                    next_val = (seq_t)candidate;
                    break;
                }
            }
            ++candidate;
        }

        cache[i] = next_val;
        ensure_used((idx_t)next_val);
        used[(idx_t)next_val] = 1;
    }

    cache_len = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* sequence defined for n ≥ 1 */

    ensure_cache(n);
    return cache[n - 1];
}
