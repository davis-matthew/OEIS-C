/*
 * A386352  Primes without {5, 8} as digits.
 * https://oeis.org/A386352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 2752, u32 n <= 2882, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_bad_digit, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386352: primes that do not contain the digits 5 or 8
 * (found by a linear search, testing each integer with is_prime_big).
 * The function returns the n‑th such prime (1‑based indexing). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/has_bad_digit.h"

/* Append a seq_t value to the dynamic array 'p'.
 * The array grows by a factor of two when full. */
static void append_seq(seq_t **p, size_t *cap, size_t *len, seq_t val) {
    if (*len >= *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*p, newcap * sizeof(seq_t));
        if (!tmp) abort();        /* unrecoverable memory failure */
        *p = tmp;
        *cap = newcap;
    }
    (*p)[(*len)++] = val;
}

/* OEIS sequence A386352. */
seq_t a(idx_t n) {
    /* Static state: array of following primes and the next integer to test. */
    static seq_t *ans = NULL;
    static size_t ans_cap = 0, ans_len = 0;
    static seq_t cur = (seq_t)2;     /* start scanning from 2 */

    /* n is 1‑based; sequences tests may not call a(0). */
    size_t target = (size_t)n;

    /* Grow the cache until we have at least 'target' applicable primes. */
    while (ans_len < target) {
        if (is_prime_big(cur)) {            /* prime test (provided by utils) */
            if (!has_bad_digit(cur)) {      /* check forbidden digits */
                append_seq(&ans, &ans_cap, &ans_len, cur);
            }
        }
        cur = cur + (seq_t)1;               /* move on to the next integer */
    }

    return ans[target - 1];
}
