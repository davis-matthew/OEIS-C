/*
 * A295614  Solution of the complementary equation a(n) = 2*a(n-1) - a(n-3) +
 *          b(n-1), where a(0) = 1, a(1) = 3, a(2) = 5, b(0) = 2, b(1) = 4,
 *          b(2) = 6, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A295614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + b(n-1); a(0)=1, a(1)=3, a(2)=5;
 *            b(0)=2, b(1)=4, b(2)=6; (a(n)) and (b(n)) are increasing
 *            complementary sequences
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* static storage for sequences */
static seq_t *a_seq = NULL;
static seq_t *b_seq = NULL;
static size_t a_cap = 0, a_len = 0;
static size_t b_cap = 0, b_len = 0;

/* Grow an array to at least 'need' elements */
static void grow_seq(seq_t **arr, size_t *cap, size_t need) {
    if (*cap >= need) return;
    *cap = (*cap == 0) ? 4 : (*cap << 1);
    *arr = (seq_t *)realloc(*arr, *cap * sizeof(seq_t));
}

/* binary search for val in a_seq[0..limit-1] */
static bool a_contains(seq_t val, size_t limit) {
    size_t lo = 0, hi = limit;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t midv = a_seq[mid];
        if (midv == val) return true;
        if (midv < val) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* Main function: returns a(n) for any n≥0 */
seq_t a(idx_t n) {
    /* Initialise on first call */
    if (a_seq == NULL) {
        a_cap = 16; b_cap = 16;
        a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));
        b_seq = (seq_t *)malloc(b_cap * sizeof(seq_t));
        /* base values */
        a_seq[0] = (seq_t)1;
        a_seq[1] = (seq_t)3;
        a_seq[2] = (seq_t)5;
        b_seq[0] = (seq_t)2;
        b_seq[1] = (seq_t)4;
        b_seq[2] = (seq_t)6;
        a_len = 3; b_len = 3;
    }

    /* If already computed, return */
    if ((size_t)n < a_len) return a_seq[n];

    /* Compute up to n */
    for (size_t i = a_len; i <= (size_t)n; ++i) {
        /* compute a(i) = 2*a(i-1) - a(i-3) + b(i-1) */
        seq_t ai = (seq_t)2 * a_seq[i - 1] - a_seq[i - 3] + b_seq[i - 1];

        grow_seq(&a_seq, &a_cap, a_len + 1);
        a_seq[a_len++] = ai;

        /* compute b(i): smallest integer > b(i-1) not in a_seq[0..a_len-1] */
        seq_t cand = b_seq[i - 1] + (seq_t)1;
        while (a_contains(cand, a_len))
            ++cand;

        grow_seq(&b_seq, &b_cap, b_len + 1);
        b_seq[b_len++] = cand;
    }

    return a_seq[n];
}
