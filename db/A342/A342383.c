/*
 * A342383  a(0) = 0; for n > 0, a(n) is the least positive integer not
 *          occurring earlier such that both the digits in a(n) and the digits
 *          in a(n-1)+a(n) are all distinct.
 * https://oeis.org/A342383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, look
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   digits_unique
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A342383: 
 * a(0) = 0; for n > 0, a(n) is the least positive integer not
 * previously used such that both a(n) and a(n-1)+a(n) have all
 * distinct decimal digits.
 * The function stores computed terms in a static cache so calls
 * with arbitrary order remain O(n) overall.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits_unique.h"

/* Dynamic cache for the sequence */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;   /* number of terms currently stored */
static size_t seq_cap = 0;   /* allocated capacity */

/* Ensure that at least n+1 elements can be stored */
static void ensure_seq_capacity(size_t n)
{
    if (n < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 4;
    while (new_cap <= n) new_cap <<= 1;
    if (seq_cache == NULL) {
        seq_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!seq_cache) abort();
    } else {
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
    }
    seq_cap = new_cap;
}

/* Check if a value has already appeared in the sequence */
static bool already_used(seq_t val)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq_cache[i] == val) return true;
    return false;
}

/* The required function */
seq_t a(idx_t n)
{
    /* Initialise the cache for the first call */
    if (seq_len == 0) {
        ensure_seq_capacity(0);
        seq_cache[0] = (seq_t)0;   /* a(0) = 0 */
        seq_len = 1;
    }

    /* Extend the cache up to index n */
    while (seq_len <= (size_t)n) {
        seq_t prev = seq_cache[seq_len - 1];
        seq_t cand = (seq_t)1;      /* start searching at 1 for each new term */

        /* Find the least unused candidate satisfying the digit rule */
        while (true) {
            if (!already_used(cand)) {
                seq_t sum = prev + cand;
                if (digits_unique(cand) && digits_unique(sum)) {
                    ensure_seq_capacity(seq_len);
                    seq_cache[seq_len] = cand;
                    ++seq_len;
                    break;
                }
            }
            cand = cand + (seq_t)1;
        }
    }

    return seq_cache[(size_t)n];
}
