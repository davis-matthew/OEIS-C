/*
 * A290025  The partial sums of 2^d(n) where d(n) is the n-th digit of the
 *          concatenated triangular numbers, and d(1)=0.
 * https://oeis.org/A290025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{k=1..n} 2^d(k), where d(k) is the k‑th digit of the
 *            concatenated triangular numbers, with d(1)=0
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   pow2, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_to_str.h"
#include "../utils/pow2.h"

static seq_t *a_cache = NULL;        /* partial sums a(1)..a(len) */
static size_t a_cache_cap = 0;       /* allocated capacity */
static size_t a_cache_len = 0;       /* number of computed terms */
static seq_t a_curr_sum = 0;         /* current cumulative sum */

/* Return the next digit of the concatenated triangular numbers. */
static uint8_t get_next_digit(void) {
    static idx_t k = 0;              /* next triangular index */
    static char buf[128];
    static size_t len = 0;           /* length of current number string */
    static size_t pos = 0;           /* current position in buf */

    if (pos >= len) {                /* need a new triangular number */
        seq_t t = ((seq_t)k * (k + 1)) / 2; /* T_k */
        size_t tmp_len;
        seq_to_str(t, buf, &tmp_len);      /* string representation */
        len = tmp_len;
        pos = 0;
        k++;
    }
    return (uint8_t)(buf[pos++] - '0');
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;       /* not used but keeps compiler happy */

    if ((idx_t)n > a_cache_len) {
        for (idx_t i = (idx_t)(a_cache_len + 1); i <= (idx_t)n; ++i) {
            uint8_t d = get_next_digit();         /* digit d(i) */
            seq_t term = pow2(d);                 /* 2^d */
            a_curr_sum += term;                   /* accumulate */

            if (a_cache_len >= a_cache_cap) {      /* grow cache if needed */
                size_t new_cap = a_cache_cap ? 2 * a_cache_cap : 4;
                seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                a_cache = tmp;
                a_cache_cap = new_cap;
            }
            a_cache[a_cache_len] = a_curr_sum;     /* store a(i) */
            a_cache_len++;
        }
    }
    return a_cache[n - 1];           /* cache is 0‑based: a_cache[0] = a(1) */
}
