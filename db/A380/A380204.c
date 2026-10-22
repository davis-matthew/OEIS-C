/*
 * A380204  A version of the Josephus problem: a(n) is the surviving integer
 *          under the spelling version of the elimination process.
 * https://oeis.org/A380204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 263, u16 n <= 9993, u32 n <= 9993, u64 n <= 9993,
 *            u128 n <= 9993, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A380204: Josephus problem where the elimination step equals
   the number of letters in the English words ONE, TWO, THREE, … for each
   successive elimination. */
#include <stdlib.h>

/* Letter counts for numbers 1..19 */
static const int unit_len[20] = {
    0, 3, 3, 5, 4, 4, 3, 5, 5, 4,
    3, 6, 6, 8, 8, 7, 7, 9, 8, 8
};

/* Letter counts for tens 20..90 */
static const int tens_len[10] = {
    0, 0, 6, 6, 5, 5, 5, 7, 6, 6
};

/* Return the number of letters in the English word for n (1 <= n <= 9999) */
static inline idx_t wordlen(idx_t n) {
    idx_t len = 0;
    if (n >= 1000) {
        idx_t thousands = n / 1000;          /* 1..9 */
        len += unit_len[thousands] + 8;     /* "thousand" */
        n %= 1000;
    }
    if (n >= 100) {
        idx_t hundreds = n / 100;           /* 1..9 */
        len += unit_len[hundreds] + 7;     /* "hundred" */
        n %= 100;
    }
    if (n >= 20) {
        idx_t tens = n / 10;                /* 2..9 */
        len += tens_len[tens];
        n %= 10;
    }
    if (n > 0) {                            /* 1..19 */
        len += unit_len[n];
    }
    return len;
}

seq_t a(idx_t n) {
    if (n == 0)            /* undefined, but guard against abuse */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Build next pointer array for a circular list 1..n */
    idx_t *next = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!next)
        return (seq_t)0;   /* allocation failed; return 0 as a sentinel */

    for (idx_t i = 1; i <= n; ++i)
        next[i] = (i == n) ? 1 : i + 1;

    idx_t current = 1;     /* starting person */
    idx_t remaining = n;
    idx_t word_index = 1;  /* ONE, TWO, THREE, ... */

    while (remaining > 1) {
        idx_t steps = wordlen(word_index);  /* letters in current word */
        word_index++;

        /* Find the node just before the one to eliminate */
        idx_t prev = current;
        for (idx_t s = 1; s < steps; ++s)
            prev = next[prev];

        idx_t elim = next[prev];

        /* Remove elim from the circle */
        next[prev] = next[elim];

        /* Next round starts at the person after the eliminated one */
        current = next[elim];
        --remaining;
    }

    free(next);
    return (seq_t)current;
}
