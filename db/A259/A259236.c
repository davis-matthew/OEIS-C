/*
 * A259236  Increasing sequence of numbers n such that the digits of n appear
 *          as a substring of the concatenation of the terms < n of the
 *          sequence, which is seeded with 1..9.
 * https://oeis.org/A259236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   A259236 – a(n) is the nth positive integer whose decimal
 *            representation appears as a substring in the concatenation of
 *            all previous terms. The sequence is seeded with 1..9. This
 *            implementation builds the sequence on demand, memoising the
 *            results
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* static storage for the sequence terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* static storage for the growing concatenation of digits
   (always null‑terminated, but concat_len excludes the terminator). */
static char *concat = NULL;
static size_t concat_len = 0;   /* number of digits stored */
static size_t concat_cap = 0;

/* smallest integer to test next (>=10) */
static uint64_t next_candidate = 10;

/* initialise global state once */
static void init_state(void)
{
    if (terms != NULL) return;   /* already initialised */

    /* seed 1..9 */
    terms_cap = 16;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    for (uint64_t i = 1; i <= 9; ++i)
        terms[terms_len++] = (seq_t)i;

    /* initialise concatenation buffer with "123456789" */
    concat_cap = 64;
    concat = (char *)malloc(concat_cap * sizeof(char));
    memcpy(concat, "123456789", 10);   /* 9 digits + '\0' */
    concat_len = 9;                   /* number of digits stored */
}

/* ensure that 'terms' has at least 'required' capacity */
static void ensure_terms_capacity(size_t required)
{
    if (terms_cap >= required) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < required) new_cap <<= 1;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* ensure that 'concat' has at least 'required' capacity */
static void ensure_concat_capacity(size_t required)
{
    if (concat_cap >= required) return;
    size_t new_cap = concat_cap ? concat_cap << 1 : 64;
    while (new_cap < required) new_cap <<= 1;
    concat = (char *)realloc(concat, new_cap * sizeof(char));
    concat_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_state();

    while (terms_len < (size_t)n) {
        /* convert candidate to decimal string */
        char num[32];
        int num_len = snprintf(num, sizeof(num), "%llu",
                               (unsigned long long)next_candidate);
        if (num_len <= 0) break;  /* safety, should not happen */

        /* check if num is substring of current concatenation */
        if (strstr(concat, num)) {
            /* append to terms array */
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)next_candidate;

            /* append digits to concatenation buffer */
            size_t required = concat_len + (size_t)num_len + 1; /* +1 for new '\0' */
            ensure_concat_capacity(required);
            memcpy(concat + concat_len, num, (size_t)num_len);
            concat_len += (size_t)num_len;
            concat[concat_len] = '\0';
        }

        ++next_candidate;
    }

    return terms[(size_t)n - 1];
}
