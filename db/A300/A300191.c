/*
 * A300191  Look and Say the digits of the last three terms, by increasing
 *          digit value. Start with a(1)=1, a(2)=2 and a(3)=3.
 * https://oeis.org/A300191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 6, u64 n <= 16, u128 n <= 3200,
 *            big any n
 * Helpers:   digit_frequencies, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A300191: Look‑and‑Say the digits of the last three terms, sorted by digit.
   Recurrence: for n>3, let C[d] be the total count of digit d in
   a(n‑3), a(n‑2), a(n‑1).  Then a(n) = concat_{d=0..9, C[d]>0}(C[d]d).
   Initial terms: a(1)=1, a(2)=2, a(3)=3. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_frequencies.h" /* void digit_frequencies(seq_t x, unsigned int freq[10]); */
#include "../utils/pow10.h"             /* seq_t pow10(idx_t exp); */
#include "../utils/digit_len.h"         /* int digit_len(seq_t x); */

static seq_t *cache = NULL;   /* cache[1..] holds computed terms, cache[0] unused */
static size_t cache_cap = 0;  /* allocated size of cache (including unused 0‑th slot) */
static idx_t   cur_max  = 0;  /* highest n for which cache[n] is computed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but guard */

    /* Allocate cache large enough for indices up to n (1‑based). */
    size_t need = (size_t)n + 1;              /* indices 0..n */
    if (cache == NULL) {
        if (need < 4) need = 4;               /* room for the first three terms */
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        cache_cap = need;
    } else if (need > cache_cap) {
        cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache_cap = need;
    }

    /* Initialize first three terms once. */
    if (cur_max == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cur_max = 3;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        seq_t cnt[10] = {0};

        /* Accumulate digit counts from the last three terms. */
        for (idx_t j = i - 3; j <= i - 1; ++j) {
            unsigned int term_cnt[10];
            digit_frequencies(cache[j], term_cnt);
            for (int d = 0; d < 10; ++d) {
                cnt[d] += (seq_t)term_cnt[d];
            }
        }

        /* Build the new term by concatenating count then digit for each non‑zero count. */
        seq_t new_term = (seq_t)0;
        for (int d = 0; d < 10; ++d) {
            if (cnt[d] == 0) continue;
            seq_t count_seq = cnt[d];
            seq_t mult = pow10((idx_t)(digit_len(count_seq) + 1));
            seq_t add  = count_seq * (seq_t)10 + (seq_t)d;
            new_term = new_term * mult + add;
        }
        cache[i] = new_term;
    }

    cur_max = n < cur_max ? cur_max : n;
    return cache[n];
}
