/*
 * A317193  First differences of A317192.
 * https://oeis.org/A317193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9929, i16 n <= 9929, i32 n <= 9929, i64 n <= 9929,
 *            i128 n <= 9929, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Sequence: First differences of A317192.
   Generated from the tribonacci morphism
      a → ab
      b → ac
      c → a
   with the mapping a→1, b→‑1, c→0.  The values are returned
   for arbitrary n≥1 and stored in a static cache so that later
   calls are O(1). */

static seq_t   *gend_cache   = NULL;   /* cache of sequence values   */
static size_t   cache_len    = 0;        /* number of values computed  */
static size_t   cache_cap    = 0;        /* allocated capacity        */

static char    *gend_word    = NULL;   /* current tribonacci word   */
static size_t   word_len     = 0;        /* length of word            */

/* Expand the cache and word until at least n terms are available. */
static void
ensure_cache(idx_t n)
{
    /* Initialisation: start with the single letter 'a' */
    if (word_len == 0) {
        gend_word = (char *)malloc(1);
        if (!gend_word) abort();
        gend_word[0] = 'a';
        word_len   = 1;
    }

    /* Grow until we have at least n terms */
    while ((size_t)n > cache_len) {
        /* Compute the length of the word after the next morphism step */
        size_t new_len = 0;
        for (size_t i = 0; i < word_len; ++i) {
            char c = gend_word[i];
            if (c == 'a' || c == 'b')
                new_len += 2;
            else
                new_len += 1;
        }

        /* Allocate new word buffer */
        char *new_word = (char *)malloc(new_len);
        if (!new_word) abort();

        /* Ensure cache can hold the new terms */
        if (cache_cap < new_len) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap < new_len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(gend_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            gend_cache = tmp;
            cache_cap  = new_cap;
        }

        /* Expand the word and fill the cache simultaneously */
        size_t pos = 0;
        for (size_t i = 0; i < word_len; ++i) {
            char c = gend_word[i];
            if (c == 'a') {
                new_word[pos] = 'a';
                new_word[pos+1] = 'b';
                gend_cache[pos] =  (seq_t)1;
                gend_cache[pos+1] = (seq_t)-1;
                pos += 2;
            } else if (c == 'b') {
                new_word[pos] = 'a';
                new_word[pos+1] = 'c';
                gend_cache[pos] =  (seq_t)1;
                gend_cache[pos+1] = (seq_t)0;
                pos += 2;
            } else { /* c */
                new_word[pos] = 'a';
                gend_cache[pos] = (seq_t)1;
                pos += 1;
            }
        }

        free(gend_word);
        gend_word = new_word;
        word_len  = new_len;
        cache_len = new_len;     /* now all cache entries up to new_len are valid */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* safety, though n≥1 by definition */
    ensure_cache(n);
    return gend_cache[(size_t)n - 1];
}
