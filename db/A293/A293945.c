/*
 * A293945  Orbit of word "2" under De Mol's 5-shift tag system over the
 *          alphabet {1,2} defined in the Comments.
 * https://oeis.org/A293945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A293945: orbit of word "2" under De Mol's 5‑shift tag system */
/* The function builds the sequence iteratively, caching generated
   terms so that a(n) can be obtained in O(1) after O(n) work. */

/* Required headers for all builds */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/seq_from_str.h"   /* seq_t seq_from_str(const char *s) */

/* Cached data */
static seq_t *values = NULL;          /* numeric values of terms   */
static char **words  = NULL;          /* word strings              */
static size_t  cap   = 0;             /* current cache capacity    */
static size_t  count = 0;             /* number of terms stored    */

/* Ensure the cache can hold at least n entries (n ≥ count). */
static void ensure_cache(size_t n)
{
    if (n <= cap)
        return;

    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < n)
        new_cap = n;

    values = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
    words  = (char**)realloc(words,  new_cap * sizeof(char*));

    /* It is assumed that realloc never fails in the test harness. */
    cap = new_cap;
}

/* Generates terms up to index n (1‑based). */
static void generate_up_to(idx_t n)
{
    if (n <= count)
        return;                      /* already computed */

    /* Make sure the cache has space for all required entries. */
    ensure_cache(n);

    /* Initialize the very first word "2" if we have nothing yet. */
    if (count == 0) {
        words[0] = (char*)malloc(2);
        words[0][0] = '2';
        words[0][1] = '\0';
        values[0] = seq_from_str(words[0]);
        count = 1;
    }

    /* Produce further terms one by one. */
    for (idx_t idx = count; idx < n; ++idx) {
        const char *prev = words[idx - 1];
        size_t      prev_len = strlen(prev);

        const char *pat;
        size_t      pat_len;
        if (prev[0] == '1') {
            pat = "2112";
            pat_len = 4;
        } else {            /* starts with '2' */
            pat = "1221222";
            pat_len = 7;
        }

        /* New length = len(prev) + len(pat) – 5, but not negative. */
        int64_t tmp = (int64_t)prev_len - 5 + (int64_t)pat_len;
        if (tmp < 0)
            tmp = 0;
        size_t new_len = (size_t)tmp;

        /* Allocate space for the new word. */
        char *next = (char*)malloc(new_len + 1);
        next[new_len] = '\0';
        size_t pos = 0;

        /* Copy the tail of the previous word after the first five
           characters, if it has that many characters. */
        if (prev_len > 5) {
            size_t copy_prev = prev_len - 5;
            memcpy(next, prev + 5, copy_prev);
            pos += copy_prev;
        }

        /* Determine how many characters of the pattern are needed. */
        size_t pat_start = 0;
        if (prev_len < 5) {
            size_t skip = 5 - prev_len;
            pat_start = skip;          /* skip the first 'skip' bytes of pat */
        }
        size_t copy_pat = pat_len - pat_start;
        if (copy_pat > 0) {
            memcpy(next + pos, pat + pat_start, copy_pat);
            pos += copy_pat;
        }

        /* Store the new word and its numeric value. */
        words[idx] = next;
        values[idx] = seq_from_str(next);
    }

    count = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for the given offset; return 0 as a safety */
        return (seq_t)0;

    generate_up_to(n);
    return values[(size_t)(n) - 1];
}
