/*
 * A336514  a(1) = 1, and for any n > 0, a(n+1) is the number of k in the
 *          range 1..n such that the decimal representation of a(k) appears as
 *          a substring in the decimal representation of a(n).
 * https://oeis.org/A336514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(1) = 1; for n ≥ 1, * a(n+1) = |{ k : 1 ≤ k ≤ n, decimal(a(k))
 *            is a substring of decimal(a(n)) }| * This implementation
 *            memoises computed values and their decimal strings. * Each new
 *            term is obtained by scanning all previous terms and testing *
 *            whether the string for a(k) occurs inside the strin...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_to_str.h"
#include "../utils/digit_len.h"

static seq_t  *seq_cache = NULL;   /* cached sequence values          */
static char **str_cache = NULL;   /* cached decimal strings          */
static size_t cache_size = 0;     /* number of terms currently cached */
static size_t cache_cap  = 0;     /* allocated capacity of the caches */

/* Ensure that the caches have space for at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_seq = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_seq)
        exit(EXIT_FAILURE);
    char **new_str = (char **)realloc(str_cache, new_cap * sizeof(char *));
    if (!new_str)
        exit(EXIT_FAILURE);

    seq_cache = new_seq;
    str_cache = new_str;
    cache_cap  = new_cap;
}

/* Initialise the cache with a(1) = 1. */
static void init_cache(void)
{
    grow_cache(1);
    seq_cache[0] = (seq_t)1;           /* a(1) */
    str_cache[0] = strdup("1");        /* decimal string of a(1) */
    if (!str_cache[0])
        exit(EXIT_FAILURE);
    cache_size = 1;
}

/* Public function for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;    /* out of bounds – not expected */

    if (cache_size == 0)
        init_cache();

    while (cache_size < (size_t)n) {
        /* We are about to compute a(cache_size+1). */
        size_t prev_idx  = cache_size - 1;   /* index of a(cache_size) */
        const char *prev_str = str_cache[prev_idx];
        seq_t new_val = 0;

        /* Count how many previous terms are substrings of a(cache_size). */
        for (size_t i = 0; i <= prev_idx; ++i) {
            if (strstr(prev_str, str_cache[i]) != NULL)
                ++new_val;
        }

        grow_cache(cache_size + 1);

        seq_cache[cache_size] = new_val;   /* store the new term */

        /* Convert new_val to a decimal string and cache it. */
        int digits = digit_len(new_val);
        /* Allocate buffer larger than digits so we can null‑terminate. */
        char *buf = (char *)malloc((size_t)digits + 1);
        if (!buf)
            exit(EXIT_FAILURE);
        size_t out_len;
        seq_to_str(new_val, buf, &out_len);   /* seq_to_str writes the digits */
        buf[digits] = '\0';                    /* ensure null termination */
        str_cache[cache_size] = buf;

        ++cache_size;
    }

    return seq_cache[n - 1];
}
